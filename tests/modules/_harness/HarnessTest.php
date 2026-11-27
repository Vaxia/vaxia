<?php

namespace Vaxia\Tests\Harness;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Checks the test harness itself, and that module tests stay black-box.
 */
class HarnessTest extends VaxiaTestCase {

  /**
   * Module tests may not call Drupal or module functions: setup goes through
   * $this->site (Site.php), everything else over HTTP. That keeps them portable
   * to the Drupal 10/11 site.
   */
  public function testModuleTestsOnlyUseHttpAndTheSiteDriver() {
    $violations = array();
    $files = new \RecursiveIteratorIterator(new \RecursiveDirectoryIterator(VAXIA_TESTS_ROOT . '/modules'));
    foreach ($files as $file) {
      if (substr($file, -8) !== 'Test.php') {
        continue;
      }
      $tokens = token_get_all(file_get_contents($file));
      $prev = NULL;
      foreach ($tokens as $i => $token) {
        if (is_array($token) && in_array($token[0], array(T_WHITESPACE, T_COMMENT, T_DOC_COMMENT), TRUE)) {
          continue;
        }
        if (is_array($token) && $token[0] === T_STRING) {
          $next = $i + 1;
          while (isset($tokens[$next]) && is_array($tokens[$next]) && $tokens[$next][0] === T_WHITESPACE) {
            $next++;
          }
          $isCall = isset($tokens[$next]) && $tokens[$next] === '(';
          $isMember = is_array($prev) && in_array($prev[0], array(T_OBJECT_OPERATOR, T_DOUBLE_COLON, T_FUNCTION, T_NEW), TRUE);
          if ($isCall && !$isMember && function_exists($token[1])) {
            $fn = new \ReflectionFunction($token[1]);
            if ($fn->isUserDefined()) {
              $violations[] = basename($file) . ':' . $token[2] . ' ' . $token[1] . '()';
            }
          }
        }
        $prev = $token;
      }
    }
    $this->assertSame(array(), $violations, 'Drupal calls in module tests (use $this->site or HTTP)');
  }

  public function testAllCustomModulesEnabled() {
    $this->login($this->superuser());
    $this->get('admin/modules');
    $list = file(VAXIA_STACK_DIR . '/modules.txt', FILE_IGNORE_NEW_LINES);
    foreach ($list as $module) {
      if ($module === '' || $module[0] === '#') {
        continue;
      }
      // D7 names the checkbox modules[<package>][<module>][enable]; D11
      // modules[<module>][enable].
      $checked = $this->xpath()->query('//input[(@name="modules[' . $module . '][enable]" or (starts-with(@name, "modules[") and contains(@name, "][' . $module . '][enable]"))) and @checked]');
      $this->assertSame(1, $checked->length, "$module enabled");
    }
  }

  public function testLoginAndViewNode() {
    $account = $this->site->createUser(array('player'));
    $node = $this->site->createNode('page', array('title' => 'Harness page'));
    $this->login($account);
    $this->get('node/' . $node->nid);
    $this->assertStatus(200);
    $this->assertText('Harness page');
  }

  public function testFormSubmitSavesData() {
    $account = $this->site->createUser();
    $this->login($account);
    $this->submitForm('user/' . $account->uid . '/edit', array(
      'current_pass' => 'pass',
      'mail' => 'changed_' . $account->mail,
    ), 'Save');
    $this->assertContains('The changes have been saved.', $this->messages('status'));
    $this->get('user/' . $account->uid . '/edit');
    $this->assertSame('changed_' . $account->mail, $this->xpath()->query('//input[@name="mail"]')->item(0)->getAttribute('value'));
  }

  public function testSessionsPerUser() {
    $a = $this->site->createUser();
    $b = $this->site->createUser();
    $this->login($a);
    $this->login($b);
    $this->login($a);
    $this->get('user');
    $this->assertText($a->name);
  }

  public function testObserversReadRolesAndStatus() {
    $account = $this->site->createUser(array('player', 'voter'), array(), array('status' => 0));
    $this->assertTrue($this->hasRole($account, 'player'));
    $this->assertTrue($this->hasRole($account, 'voter'));
    $this->assertTrue($this->isBlocked($account));
  }

  public function testMailIsCollectedPerTest() {
    $account = $this->site->createUser();
    $this->anonymous();
    $this->submitForm('user/password', array('name' => $account->name), 'E-mail new password');
    $this->assertSame(array($account->mail), $this->mailRecipients());
  }

  public function testMailFromAnEarlierTestIsGone() {
    $account = $this->site->createUser();
    $this->anonymous();
    $this->submitForm('user/password', array('name' => $account->name), 'E-mail new password');
    $this->assertCount(1, $this->sentMail());
  }

  public function testWorkflowTransitionThroughTheTab() {
    $player = $this->site->createUser(array('player'));
    $character = $this->site->createCharacter($player);
    $this->transition($character, 'draft');
    $this->assertSame('draft', $this->workflowState($character));
  }

}
