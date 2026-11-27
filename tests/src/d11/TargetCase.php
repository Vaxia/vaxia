<?php

namespace Vaxia\Tests;

use Drupal\Core\Database\Database;
use Drupal\Core\Session\AnonymousUserSession;
use PHPUnit\Framework\TestCase;

/**
 * Drupal 11 specifics of the test base class: database snapshot, caches, mail,
 * cron, and how the D11 site shows messages, roles and workflow states.
 *
 * VaxiaTestCase extends this; tests never call it directly. The Drupal 7
 * counterpart is src/d7/TargetCase.php.
 */
abstract class TargetCase extends TestCase {

  abstract protected function get($path, array $query = array());

  abstract protected function submitForm($path, array $edit, $button, $formId = NULL);

  abstract protected function inSession($account, callable $fn);

  abstract protected function xpath();

  public static function setUpBeforeClass(): void {
    parent::setUpBeforeClass();
    self::restoreBaseline();
  }

  protected function targetSetUp() {
    self::resetDrupalCaches();
  }

  protected function targetTearDown() {
    \Drupal::currentUser()->setAccount(new AnonymousUserSession());
  }

  // ---------------------------------------------------------------------------
  // Database / cache state.
  // ---------------------------------------------------------------------------

  /**
   * Reloads the post-install snapshot (tests/.build/d11-baseline.sql).
   */
  protected static function restoreBaseline() {
    $connection = \Drupal::database();
    $db = Database::getConnectionInfo()['default'];
    // Drop every table first: tests can create tables the dump doesn't know.
    $tables = $connection->query('SHOW TABLES')->fetchCol();
    if ($tables) {
      $connection->query('SET FOREIGN_KEY_CHECKS = 0');
      $connection->query('DROP TABLE `' . implode('`, `', $tables) . '`');
      $connection->query('SET FOREIGN_KEY_CHECKS = 1');
    }
    $cmd = 'mariadb -h' . escapeshellarg($db['host']) . ' -u' . escapeshellarg($db['username'])
      . ' -p' . escapeshellarg($db['password']) . ' ' . escapeshellarg($db['database'])
      . ' < ' . escapeshellarg(VAXIA_BASELINE_SQL) . ' 2>&1';
    exec($cmd, $out, $code);
    if ($code !== 0) {
      throw new \RuntimeException('Baseline restore failed: ' . implode("\n", $out));
    }
    // Routes live in a table the snapshot keeps: rebuild them from the current
    // code (config changes still need tests/docker-d11/install.sh).
    \Drupal::service('router.builder')->rebuild();
    self::resetDrupalCaches();
  }

  /**
   * Drops in-process caches so reads see what Apache wrote.
   */
  protected static function resetDrupalCaches() {
    drupal_static_reset();
    \Drupal::service('entity.memory_cache')->deleteAll();
    $manager = \Drupal::entityTypeManager();
    foreach ($manager->getDefinitions() as $id => $definition) {
      if ($manager->hasHandler($id, 'storage')) {
        $manager->getStorage($id)->resetCache();
      }
    }
    \Drupal::configFactory()->reset();
    \Drupal::state()->resetCache();
  }

  // ---------------------------------------------------------------------------
  // How the site talks back.
  // ---------------------------------------------------------------------------

  /**
   * URL that answers a form's #ajax requests: D11 posts back to the form's own
   * URL with the AJAX wrapper format.
   */
  protected function ajaxUrl($formAction) {
    return $formAction . (strpos($formAction, '?') === FALSE ? '?' : '&') . 'ajax_form=1&_wrapper_format=drupal_ajax';
  }

  /**
   * Core buttons D11 labels differently. VAXIA's own buttons keep their D7
   * labels when their modules are ported.
   */
  private const BUTTON_LABELS = array(
    'E-mail new password' => 'Submit',
    'Save content type' => 'Save',
  );

  /**
   * Path on this site for a path written the D7 way: content type admin pages
   * use the machine name (rpg_chatroom), not D7's hyphenated form; the role
   * list moved out from under permissions.
   */
  protected function sitePath($path) {
    $path = preg_replace('#^(/?)admin/people/permissions/roles$#', '$1admin/people/roles', $path);
    $path = preg_replace_callback('#^(/?node/add/)([a-z0-9-]+)#', function ($m) {
      return $m[1] . str_replace('-', '_', $m[2]);
    }, $path);
    return preg_replace_callback('#^(/?admin/structure/types/manage/)([a-z0-9-]+)#', function ($m) {
      return $m[1] . str_replace('-', '_', $m[2]);
    }, $path);
  }

  /**
   * Other names a field may have on a D11 form: single-value base fields
   * carry a delta (title[0][value]); reference boxes take [target_id], with
   * or without a delta.
   */
  protected function fieldNameCandidates($name) {
    // Single checkboxes (boolean fields) take [value].
    $candidates = array($name . '[0][value]', $name . '[value]');
    // privatemsg 2.x's new-message form.
    $renamed = array('recipient' => 'thread_members', 'subject' => 'thread_subject', 'body[value]' => 'message[0][value]', 'log' => 'revision_log[0][value]');
    if (isset($renamed[$name])) {
      $candidates[] = $renamed[$name];
    }
    if (preg_match('/^(\w+)\[0\]\[(uid|nid|tid|target_id)\]$/', $name, $m)) {
      $candidates[] = $m[1] . '[target_id]';
      $candidates[] = $m[1] . '[0][target_id]';
    }
    return $candidates;
  }

  /**
   * XPath of a collapsed fieldset (D7 fieldset.collapsed, D11 closed details).
   */
  protected function collapsedXpath() {
    return '//details[not(@open)]';
  }

  /**
   * XPath of a webform's submission form on the page.
   */
  protected function webformFormXpath() {
    return '//form[contains(concat(" ", @class, " "), " webform-submission-form ")]';
  }

  /**
   * What to type in a reference autocomplete for an entity: "label (id)".
   */
  protected function referenceValue($label, $id, $type = 'node') {
    return $label . ' (' . $id . ')';
  }

  /**
   * Form values for a date field (D11 date box).
   */
  protected function dateEdit($name, $timestamp) {
    return array($name . '[0][value][date]' => date('Y-m-d', $timestamp));
  }

  /**
   * Button label on this site for a label written the D7 way.
   */
  protected function buttonLabel($label) {
    return self::BUTTON_LABELS[$label] ?? $label;
  }

  /**
   * Form field name on this site for a name written the D7 way:
   * - no language level: field_x[und][0][value] -> field_x[0][value];
   * - field collections are paragraphs, whose fields sit in a subform:
   *   field_skill[0][field_skill_name][0][value]
   *   -> field_skill[0][subform][field_skill_name][0][value].
   */
  protected function fieldName($name) {
    $name = str_replace('[und]', '', $name);
    // D7 webform named elements submitted[key]; Webform 6 uses the key.
    $name = preg_replace('/^submitted\[(\w+)\]/', '$1', $name);
    return preg_replace('/^(field_\w+)\[(\d+)\]\[(field_\w+)\]/', '$1[$2][subform][$3]', $name);
  }

  /**
   * Logs the current session out (D11 asks to confirm).
   */
  protected function logout() {
    $this->submitForm('user/logout', array(), 'Log out');
    if ($this->loggedInUser) {
      $this->forgetSession($this->loggedInUser);
    }
    $this->loggedInUser = NULL;
  }

  /**
   * Drupal status/error/warning messages shown on the last page.
   */
  protected function messages($type = NULL) {
    $class = $type ? ' messages--' . $type . ' ' : ' messages ';
    $xpath = $this->xpath();
    $out = array();
    foreach ($xpath->query('//div[contains(concat(" ", normalize-space(@class), " "), "' . $class . '")]') as $div) {
      $content = $xpath->query('.//div[contains(@class, "messages__content")]', $div)->item(0) ?: $div;
      foreach ($xpath->query('.//*[contains(@class, "visually-hidden")]', $content) as $hidden) {
        $hidden->parentNode->removeChild($hidden);
      }
      $items = $xpath->query('.//li', $content);
      if ($items->length) {
        foreach ($items as $li) {
          $out[] = trim(preg_replace('/\s+/', ' ', $li->textContent));
        }
      }
      else {
        $out[] = trim(preg_replace('/\s+/', ' ', $content->textContent));
      }
    }
    return $out;
  }

  /**
   * There is no Rules engine on D11: its logic is PHP code (decided).
   *
   * @return string[]
   */
  protected function rulesErrors() {
    return array();
  }

  protected function assertNoRulesErrors() {
    $this->assertSame(array(), $this->rulesErrors(), 'Rules evaluation errors');
  }

  // ---------------------------------------------------------------------------
  // Mail and cron.
  // ---------------------------------------------------------------------------

  protected function clearMail() {
    \Drupal::state()->set('system.test_mail_collector', array());
  }

  /**
   * Mails sent so far by Apache or in-process code (test_mail_collector).
   */
  protected function sentMail() {
    // Read the row itself: the in-process state cache may predate Apache's write.
    $raw = \Drupal::database()->query("SELECT value FROM {key_value} WHERE collection = 'state' AND name = 'system.test_mail_collector'")->fetchField();
    return $raw ? unserialize($raw, array('allowed_classes' => FALSE)) : array();
  }

  /**
   * Runs cron the way the server's crontab does (GET /cron/<key>).
   */
  protected function runCron() {
    $key = \Drupal::state()->get('system.cron_key');
    $this->inSession(NULL, function () use ($key) {
      $this->get('cron/' . $key);
    });
  }

  // ---------------------------------------------------------------------------
  // Observing accounts and workflow over HTTP.
  // ---------------------------------------------------------------------------

  /**
   * Role names an account holds, read from its admin edit form.
   *
   * @return string[]
   *   Sorted role labels, without "authenticated user".
   */
  protected function rolesOf($account) {
    return $this->inSession($this->superuser(), function () use ($account) {
      $this->get('user/' . $account->uid . '/edit');
      $xpath = $this->xpath();
      $names = array();
      foreach ($xpath->query('//input[starts-with(@name, "roles[") and @checked and not(@disabled)]') as $input) {
        $label = $xpath->query('//label[@for="' . $input->getAttribute('id') . '"]')->item(0);
        $name = $label ? trim($label->textContent) : '';
        if ($name !== '' && $name !== 'authenticated user') {
          $names[] = $name;
        }
      }
      sort($names);
      return $names;
    });
  }

  /**
   * Whether a role holds a permission, read from the permissions page.
   */
  protected function roleHasPermission($roleName, $permission) {
    $rid = $this->site->roleId($roleName);
    return $this->inSession($this->superuser(), function () use ($rid, $permission) {
      $this->get('admin/people/permissions/' . $rid);
      return $this->xpath()->query('//input[@name="' . $rid . '[' . $permission . ']" and @checked]')->length === 1;
    });
  }

  /**
   * Whether an account is blocked, read from its admin edit form.
   */
  protected function isBlocked($account) {
    return $this->inSession($this->superuser(), function () use ($account) {
      $this->get('user/' . $account->uid . '/edit');
      return $this->xpath()->query('//input[@name="status" and @value="0" and @checked]')->length === 1;
    });
  }

  /**
   * State choices offered on the Workflow tab of the last loaded page.
   */
  protected function workflowChoices() {
    $xpath = $this->xpath();
    $choices = array();
    foreach ($xpath->query('//input[@type="radio" and contains(@name, "[to_sid]")]') as $radio) {
      $label = $xpath->query('//label[@for="' . $radio->getAttribute('id') . '"]')->item(0);
      $choices[] = trim($label->textContent);
    }
    return $choices;
  }

  /**
   * A node's workflow state name, from its page (the D11 Workflow tab shows
   * only the transition form and history; the node shows the state field).
   */
  protected function workflowState($node) {
    return $this->inSession($this->superuser(), function () use ($node) {
      $this->get('node/' . $node->nid);
      $item = $this->xpath()->query('//div[contains(@class, "field--type-workflow")]//div[contains(@class, "field__item")]')->item(0);
      return $item ? trim($item->textContent) : NULL;
    });
  }

  /**
   * Moves a node to a workflow state through its Workflow tab.
   *
   * @param object|null $account
   *   Who presses the button; defaults to the superuser, who may make any move.
   *
   * @return string[]
   *   Messages shown after the move.
   */
  protected function transition($node, $state, $account = NULL, $comment = '') {
    $this->site->ageWorkflowHistory($node);
    return $this->inSession($account ?: $this->superuser(), function () use ($node, $state, $comment) {
      $this->get('node/' . $node->nid . '/workflow');
      $xpath = $this->xpath();
      $field = NULL;
      $value = NULL;
      foreach ($xpath->query('//input[@type="radio" and contains(@name, "[to_sid]")]') as $radio) {
        $label = $xpath->query('//label[@for="' . $radio->getAttribute('id') . '"]')->item(0);
        if ($label && trim($label->textContent) === $state) {
          $field = $radio->getAttribute('name');
          $value = $radio->getAttribute('value');
        }
      }
      $this->assertNotNull($value, "Workflow state '$state' offered for node {$node->nid} on {$this->url}");
      $edit = array($field => $value);
      if ($comment !== '') {
        $edit[str_replace('[to_sid]', '[comment][0][value]', $field)] = $comment;
      }
      $this->submitForm(NULL, $edit, 'Update workflow');
      return $this->messages();
    });
  }

}
