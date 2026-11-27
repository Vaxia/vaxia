<?php

namespace Vaxia\Tests;

/**
 * Base class for VAXIA module tests.
 *
 * Tests are black-box so they survive the move to Drupal 10/11:
 * - ARRANGE through $this->site (Site.php), the only code that touches Drupal.
 * - ACT over HTTP: pages, forms, the workflow tab, cron.php.
 * - ASSERT over HTTP: page text, messages, admin screens, captured mail.
 * HarnessTest fails if a test file calls Drupal or the database directly.
 *
 * The database is restored from the target's baseline snapshot once per test
 * class. Site-version specifics (D7 or D11, picked by VAXIA_TARGET) live in
 * src/<target>/TargetCase.php and src/<target>/Site.php.
 */
abstract class VaxiaTestCase extends TargetCase {

  /** @var string Raw body of the last HTTP response. */
  protected $content = '';

  /** @var int Status code of the last HTTP response. */
  protected $status = 0;

  /** @var string Final URL of the last HTTP response (after redirects). */
  protected $url = '';

  /** @var string|null Cookie jar file for the logged-in HTTP session. */
  protected $cookieJar;

  /** @var object|null Account logged in over HTTP in the current session. */
  protected $loggedInUser;

  /** @var array Cookie jar per account name, so tests can switch users. */
  private $sessions = array();

  /** @var int Seconds before an HTTP request is abandoned. */
  protected $httpTimeout = 120;

  /** @var Site Setup driver (the port layer). */
  protected $site;

  /** @var string[] PHP errors printed on pages during the current test. */
  protected $pageErrorsThisTest = array();

  protected function setUp(): void {
    parent::setUp();
    $this->pageErrorsThisTest = array();
    $this->site = new Site();
    $this->targetSetUp();
    $this->site->expireViewsCache();
    $this->clearMail();
  }

  protected function tearDown(): void {
    $this->targetTearDown();
    foreach ($this->sessions as $session) {
      if (is_file($session['jar'])) {
        unlink($session['jar']);
      }
    }
    if ($this->cookieJar && is_file($this->cookieJar)) {
      unlink($this->cookieJar);
    }
    $this->sessions = array();
    $this->cookieJar = NULL;
    $this->loggedInUser = NULL;
    parent::tearDown();
  }

  // ---------------------------------------------------------------------------
  // HTTP.
  // ---------------------------------------------------------------------------

  /**
   * Builds an absolute URL for a Drupal path ("node/1", "/user", or "http://...").
   */
  protected function buildUrl($path, array $query = array()) {
    if (preg_match('#^https?://#', $path)) {
      $url = $path;
    }
    else {
      $url = VAXIA_TEST_BASE_URL . '/' . ltrim($this->sitePath($path), '/');
    }
    if ($query) {
      $url .= (strpos($url, '?') === FALSE ? '?' : '&') . http_build_query($query);
    }
    return $url;
  }

  /**
   * Performs an HTTP request; stores status, body and final URL.
   */
  protected function request($method, $path, $body = NULL, array $query = array(), array $headers = array()) {
    if (!$this->cookieJar) {
      $this->cookieJar = tempnam(sys_get_temp_dir(), 'vaxia_cookies_');
    }
    $ch = curl_init($this->buildUrl($path, $query));
    curl_setopt_array($ch, array(
      CURLOPT_RETURNTRANSFER => TRUE,
      CURLOPT_FOLLOWLOCATION => TRUE,
      CURLOPT_MAXREDIRS => 10,
      CURLOPT_COOKIEJAR => $this->cookieJar,
      CURLOPT_COOKIEFILE => $this->cookieJar,
      CURLOPT_CUSTOMREQUEST => $method,
      CURLOPT_HTTPHEADER => $headers,
      CURLOPT_TIMEOUT => $this->httpTimeout,
    ));
    if ($body !== NULL) {
      curl_setopt($ch, CURLOPT_POSTFIELDS, $body);
    }
    $this->content = (string) curl_exec($ch);
    $this->status = (int) curl_getinfo($ch, CURLINFO_RESPONSE_CODE);
    $this->url = (string) curl_getinfo($ch, CURLINFO_EFFECTIVE_URL);
    $err = curl_error($ch);
    curl_close($ch);
    if ($err) {
      throw new \RuntimeException("HTTP $method $path failed: $err");
    }
    $this->logPhpErrors($method, $path);
    // Apache may have written to the DB; drop in-process caches.
    self::resetDrupalCaches();
    return $this->content;
  }

  /**
   * PHP errors Drupal printed on the last page (it shows them as error messages).
   */
  protected function phpErrorsOnPage() {
    preg_match_all('#(?:Notice|Warning|Deprecated|Error|Fatal error|Recoverable fatal error|User error|User warning|User notice)(?:</em>)?: .*?\(line <em[^>]*>\d+</em> of <em[^>]*>[^<]+</em>\)#s', $this->content, $m);
    return array_map(function ($e) {
      return html_entity_decode(strip_tags($e), ENT_QUOTES, 'UTF-8');
    }, $m[0]);
  }

  /**
   * Appends PHP errors seen on pages to tests/.build/php-errors.log, so they can be
   * reported as bugs without failing tests about other behaviour.
   */
  private function logPhpErrors($method, $path) {
    $errors = $this->phpErrorsOnPage();
    if (!$errors) {
      return;
    }
    $this->pageErrorsThisTest = array_merge($this->pageErrorsThisTest, $errors);
    $line = static::class . '::' . $this->getName() . " $method $path\n";
    foreach (array_unique($errors) as $e) {
      $line .= "  $e\n";
    }
    file_put_contents(VAXIA_TESTS_ROOT . '/.build/php-errors.log', $line, FILE_APPEND);
  }

  protected function assertNoPhpErrors() {
    $this->assertSame(array(), $this->phpErrorsOnPage(), "PHP errors on {$this->url}");
  }

  protected function get($path, array $query = array()) {
    return $this->request('GET', $path, NULL, $query);
  }

  /**
   * Switches to $account's browser session, logging in through the login form
   * the first time. Each account keeps its own cookies, so tests can hop between
   * users without logging out.
   */
  protected function login($account) {
    $key = $account->name;
    if (isset($this->sessions[$key])) {
      $this->cookieJar = $this->sessions[$key]['jar'];
      $this->loggedInUser = $account;
      return;
    }
    $this->cookieJar = tempnam(sys_get_temp_dir(), 'vaxia_cookies_');
    $this->sessions[$key] = array('jar' => $this->cookieJar);
    $this->submitForm('user/login', array(
      'name' => $account->name,
      'pass' => $account->pass_raw ?? 'pass',
    ), 'Log in');
    $this->assertStringContainsString('user/logout', $this->content, "Logged in as {$account->name}");
    $this->loggedInUser = $account;
  }

  /**
   * Switches to a fresh anonymous browser session.
   */
  protected function anonymous() {
    $this->cookieJar = tempnam(sys_get_temp_dir(), 'vaxia_cookies_');
    $this->loggedInUser = NULL;
  }

  /**
   * Forgets an account's browser session (after it logged out).
   */
  protected function forgetSession($account) {
    unset($this->sessions[$account->name]);
  }

  /**
   * The site's superuser (uid 1, created by install.sh as admin / admin).
   */
  protected function superuser() {
    return (object) array('uid' => 1, 'name' => 'admin', 'pass_raw' => 'admin');
  }

  /**
   * Runs $fn in $account's session, then restores the current session and the
   * last response, so observing doesn't disturb the page under test.
   */
  protected function inSession($account, callable $fn) {
    $saved = array($this->cookieJar, $this->loggedInUser, $this->content, $this->status, $this->url);
    $account ? $this->login($account) : $this->anonymous();
    try {
      return $fn();
    }
    finally {
      list($this->cookieJar, $this->loggedInUser, $this->content, $this->status, $this->url) = $saved;
    }
  }

  /**
   * Loads a page, fills in a form and submits it, like a browser would.
   *
   * @param string|null $path
   *   Page holding the form; NULL uses the last loaded page.
   * @param array $edit
   *   Field name => value. Checkbox: TRUE/FALSE. Multi-select: array of values.
   * @param string $button
   *   Value (label) of the submit button to press.
   * @param string|null $formId
   *   HTML id of the form, when the page has more than one.
   */
  /**
   * Runs $fn with $permission revoked from $roles (production grants many
   * permissions to anonymous / authenticated / player), then restores it.
   */
  protected function withoutPermission($permission, array $roles, callable $fn) {
    $had = array();
    foreach ($roles as $role) {
      if ($this->site->roleHas($role, $permission)) {
        $had[] = $role;
        $this->site->revokePermissions($role, array($permission));
      }
    }
    try {
      return $fn();
    }
    finally {
      foreach ($had as $role) {
        $this->site->grantPermissions($role, array($permission));
      }
    }
  }

  protected function submitForm($path, array $edit, $button, $formId = NULL) {
    $button = $this->buttonLabel($button);
    if ($path !== NULL) {
      $this->get($path);
    }
    list($action, $pairs) = $this->formPayload($edit, $button, $formId);
    $pairs[] = 'op=' . rawurlencode($button);
    return $this->request('POST', $action, implode('&', $pairs), array(), array('Content-Type: application/x-www-form-urlencoded'));
  }

  /**
   * Fires a form element's #ajax, as a browser does when $triggerName changes,
   * then applies the returned commands to the current page ("insert" replaces
   * markup, "update_build_id" swaps the form build id). A later submitForm() then
   * sees the rebuilt form. Returns the decoded commands.
   *
   * @param string $button
   *   A submit button of the form, to find it.
   */
  protected function triggerAjax($button, array $edit, $triggerName, $formId = NULL) {
    $button = $this->buttonLabel($button);
    list($action, $pairs) = $this->formPayload($edit, $button, $formId);
    $pairs[] = '_triggering_element_name=' . rawurlencode($this->fieldName($triggerName));
    $page = array($this->content, $this->status, $this->url);
    $this->request('POST', $this->ajaxUrl($action), implode('&', $pairs), array(), array('Content-Type: application/x-www-form-urlencoded', 'X-Requested-With: XMLHttpRequest'));
    $status = $this->status;
    $commands = json_decode($this->content, TRUE);
    list($this->content, $this->status, $this->url) = $page;
    $this->assertSame(200, $status, 'AJAX request succeeded');
    $this->assertIsArray($commands, 'AJAX returned commands');

    $doc = new \DOMDocument();
    libxml_use_internal_errors(TRUE);
    $doc->loadHTML($this->content);
    $xpath = new \DOMXPath($doc);
    foreach ($commands as $command) {
      if (($command['command'] ?? '') === 'update_build_id') {
        foreach ($xpath->query('//input[@name="form_build_id" and @value="' . $command['old'] . '"]') as $input) {
          $input->setAttribute('value', $command['new']);
        }
      }
      if (($command['command'] ?? '') === 'insert') {
        // Browsers call $wrapper[method]() (jQuery): a method jQuery doesn't
        // have, such as D7's "replace", fails there with "is not a function".
        if (isset($command['method'])) {
          // (D7's ajax.js mapped "replace" itself.)
          $methods = array('replaceWith', 'replaceAll', 'html', 'prepend', 'append', 'before', 'after');
          if (getenv('VAXIA_TARGET') === 'd7') {
            $methods[] = 'replace';
          }
          $this->assertContains($command['method'], $methods, 'AJAX insert method is a jQuery method');
        }
        $fragment = new \DOMDocument();
        $fragment->loadHTML('<?xml encoding="UTF-8"><div id="vaxia-ajax-fragment">' . $command['data'] . '</div>');
        $wrapper = $fragment->getElementById('vaxia-ajax-fragment');
        // No selector means "the element's #ajax wrapper", which a browser knows
        // from the page settings; the returned markup carries that same id.
        $id = NULL;
        if (!empty($command['selector']) && preg_match('/^#([\w-]+)$/', $command['selector'], $m)) {
          $id = $m[1];
        }
        elseif (empty($command['selector'])) {
          $top = (new \DOMXPath($fragment))->query('*[@id]', $wrapper)->item(0);
          $id = $top ? $top->getAttribute('id') : NULL;
        }
        $target = $id ? $xpath->query('//*[@id="' . $id . '"]')->item(0) : NULL;
        if (!$target) {
          continue;
        }
        $method = $command['method'] ?: 'replaceWith';
        if (in_array($method, array('replace', 'replaceWith'), TRUE)) {
          foreach (iterator_to_array($wrapper->childNodes) as $child) {
            $target->parentNode->insertBefore($doc->importNode($child, TRUE), $target);
          }
          $target->parentNode->removeChild($target);
        }
        else {
          while ($target->firstChild) {
            $target->removeChild($target->firstChild);
          }
          foreach (iterator_to_array($wrapper->childNodes) as $child) {
            $target->appendChild($doc->importNode($child, TRUE));
          }
        }
      }
    }
    libxml_clear_errors();
    $this->content = $doc->saveHTML();
    return $commands;
  }

  /**
   * The form with $button (or $formId) on the current page, with $edit applied:
   * array(action URL, urlencoded name=value pairs).
   */
  private function formPayload(array $edit, $button, $formId) {
    $xpath = $this->xpath();
    $forms = $formId
      ? $xpath->query('//form[@id="' . $formId . '"]')
      : $xpath->query('//form[.//input[@type="submit" and @value="' . $button . '"] or .//button[@value="' . $button . '"]]');
    $this->assertGreaterThan(0, $forms->length, "Form with button '$button' found on " . $this->url);
    $form = $forms->item(0);

    $fields = $this->formDefaults($xpath, $form);
    foreach ($edit as $name => $value) {
      $name = $this->fieldName($name);
      // The site may name it differently (title[0][value], a reference box's
      // [target_id]): take the first candidate the form has.
      if (!array_key_exists($name, $fields)) {
        foreach ($this->fieldNameCandidates($name) as $candidate) {
          if (array_key_exists($candidate, $fields)) {
            $name = $candidate;
            break;
          }
        }
      }
      $this->assertArrayHasKey($name, $fields, "Field '$name' exists on form");
      if (is_bool($value)) {
        if ($value) {
          $fields[$name] = $fields[$name]['on'] ?? '1';
        }
        else {
          $fields[$name] = NULL;
        }
        continue;
      }
      $fields[$name] = $value;
    }
    $pairs = array();
    foreach ($fields as $name => $value) {
      if (is_array($value) && array_key_exists('on', $value)) {
        // Checkbox left at its default.
        $value = $value['checked'] ? $value['on'] : NULL;
      }
      if ($value === NULL) {
        continue;
      }
      foreach ((array) $value as $v) {
        $pairs[] = rawurlencode($name) . '=' . rawurlencode($v);
      }
    }

    $action = $form->getAttribute('action') ?: $this->url;
    if (strpos($action, 'http') !== 0) {
      $action = VAXIA_TEST_BASE_URL . '/' . ltrim($action, '/');
    }
    return array($action, $pairs);
  }

  /**
   * Current values of every field in a form.
   */
  private function formDefaults(\DOMXPath $xpath, \DOMElement $form) {
    $fields = array();
    // Browsers don't submit disabled controls.
    foreach ($xpath->query('.//input[not(@disabled)]', $form) as $input) {
      $name = $input->getAttribute('name');
      $type = strtolower($input->getAttribute('type') ?: 'text');
      if ($name === '' || in_array($type, array('submit', 'button', 'image', 'reset', 'file'), TRUE)) {
        continue;
      }
      if ($type === 'checkbox') {
        $fields[$name] = array('on' => $input->getAttribute('value') ?: '1', 'checked' => $input->hasAttribute('checked'));
      }
      elseif ($type === 'radio') {
        if ($input->hasAttribute('checked')) {
          $fields[$name] = $input->getAttribute('value');
        }
        elseif (!array_key_exists($name, $fields)) {
          $fields[$name] = NULL;
        }
      }
      else {
        $fields[$name] = $input->getAttribute('value');
      }
    }
    foreach ($xpath->query('.//textarea[not(@disabled)]', $form) as $ta) {
      $fields[$ta->getAttribute('name')] = $ta->textContent;
    }
    foreach ($xpath->query('.//select[not(@disabled)]', $form) as $select) {
      $name = $select->getAttribute('name');
      $selected = array();
      foreach ($xpath->query('.//option[@selected]', $select) as $opt) {
        $selected[] = $opt->getAttribute('value');
      }
      if (!$selected && !$select->hasAttribute('multiple')) {
        $first = $xpath->query('.//option', $select)->item(0);
        $selected = $first ? array($first->getAttribute('value')) : array();
      }
      $fields[$name] = $select->hasAttribute('multiple') ? $selected : ($selected[0] ?? NULL);
    }
    return $fields;
  }

  protected function xpath() {
    $doc = new \DOMDocument();
    libxml_use_internal_errors(TRUE);
    $doc->loadHTML($this->content ?: '<html></html>');
    libxml_clear_errors();
    return new \DOMXPath($doc);
  }

  /**
   * Visible text of the last page (tags stripped, whitespace collapsed).
   */
  protected function pageText() {
    $text = preg_replace('#<(script|style)\b.*?</\1>#is', ' ', $this->content);
    // Collapse whitespace, including the non-breaking spaces field labels use.
    return trim(preg_replace('/[\s\x{00A0}]+/u', ' ', html_entity_decode(strip_tags($text), ENT_QUOTES, 'UTF-8')));
  }

  protected function assertStatus($code) {
    $this->assertSame($code, $this->status, "HTTP status for {$this->url}");
  }

  protected function assertText($text) {
    $this->assertTrue(strpos($this->pageText(), $text) !== FALSE, "'$text' expected on {$this->url}");
  }

  /**
   * $text appears in the page source (scripts and settings included).
   */
  protected function assertRaw($text) {
    $this->assertStringContainsString($text, $this->content, "'$text' expected in the source of " . $this->url);
  }

  protected function assertNoRaw($text) {
    $this->assertStringNotContainsString($text, $this->content, "'$text' not expected in the source of " . $this->url);
  }

  protected function assertNoText($text) {
    $this->assertTrue(strpos($this->pageText(), $text) === FALSE, "'$text' not expected on {$this->url}");
  }

  // ---------------------------------------------------------------------------
  // Observing the site over HTTP.
  // ---------------------------------------------------------------------------

  /**
   * Addresses that received mail so far.
   */
  protected function mailRecipients() {
    return array_map(function ($m) {
      return $m['to'];
    }, $this->sentMail());
  }

  /**
   * Id of the first post (comment) on the last page: D7 marks posts with
   * <a id="comment-N">, D11 with <article id="comment-N">.
   */
  protected function firstCommentId() {
    foreach ($this->xpath()->query('//*[starts-with(@id, "comment-")]') as $element) {
      if (preg_match('/^comment-(\d+)$/', $element->getAttribute('id'), $m)) {
        return $m[1];
      }
    }
    return NULL;
  }

  protected function hasRole($account, $role) {
    return in_array($role, $this->rolesOf($account), TRUE);
  }

  /**
   * Visible text of an account's profile page, as staff see it.
   */
  protected function profileText($account) {
    return $this->inSession($this->superuser(), function () use ($account) {
      $this->get('user/' . $account->uid);
      return $this->pageText();
    });
  }

  /**
   * Visible text of a node's page, as staff see it.
   */
  protected function nodeText($node) {
    return $this->inSession($this->superuser(), function () use ($node) {
      $this->get('node/' . $node->nid);
      return $this->pageText();
    });
  }

  /**
   * Walks a character to "approved" (rules may auto-advance it on the way).
   */
  protected function approveCharacter($character, $account = NULL) {
    foreach (array('draft', 'submitted', 'approved') as $state) {
      if ($this->workflowState($character) === 'approved') {
        return;
      }
      if ($this->workflowState($character) !== $state) {
        $this->transition($character, $state, $account);
      }
    }
    $this->assertSame('approved', $this->workflowState($character));
  }

  /**
   * Option value for a visible label in a select or radio set on the last page.
   */
  protected function optionValue($name, $label) {
    $xpath = $this->xpath();
    foreach ($xpath->query('//select[@name="' . $name . '"]/option') as $opt) {
      if (trim($opt->textContent) === $label) {
        return $opt->getAttribute('value');
      }
    }
    foreach ($xpath->query('//input[@name="' . $name . '"]') as $input) {
      $l = $xpath->query('//label[@for="' . $input->getAttribute('id') . '"]')->item(0);
      if ($l && trim($l->textContent) === $label) {
        return $input->getAttribute('value');
      }
    }
    $this->fail("No option '$label' for '$name' on {$this->url}");
  }

  /**
   * PHP errors on pages this test loaded that come from one source file.
   */
  protected function pageErrorsFrom($file) {
    return array_values(array_unique(array_filter($this->pageErrorsThisTest, function ($e) use ($file) {
      return strpos($e, $file) !== FALSE;
    })));
  }

}
