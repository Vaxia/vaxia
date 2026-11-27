<?php

namespace Vaxia\Tests\Rpg\Vacation;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Vacation: staff put themselves on vacation from their profile edit page, which
 * parks their roles (keeping player + "vacation mode") until they return or cron
 * ends the vacation.
 *
 * @group vacation
 */
class VacationTest extends VaxiaTestCase {

  private function createStaff() {
    return $this->site->createUser(array('storyteller'), array('can go on vacation'));
  }

  private function goOnVacation($account, $duration = '1 week') {
    $this->login($account);
    $this->submitForm('user/' . $account->uid . '/edit', array('vacation_end' => $duration), 'Go on vacation');
  }

  private function onVacation($account) {
    return $this->hasRole($account, 'vacation mode');
  }

  /**
   * Ends today at noon: inside the window cron looks at.
   */
  private function endsToday($account) {
    $this->site->setVacationEnd($account, strtotime(date('m/d/Y') . ' 12:00:00'));
  }

  // ---------------------------------------------------------------------------
  // Settings.
  // ---------------------------------------------------------------------------

  public function testAdminSetsTextShownOnProfile() {
    $this->login($this->site->createUser(array(), array('administer vacation')));
    $this->submitForm('admin/rpg/vacation', array('vacation_description' => 'Gone fishing rules'), 'Save configuration');
    $this->assertContains('The configuration options have been saved.', $this->messages('status'));

    $staff = $this->createStaff();
    $this->login($staff);
    $this->get('user/' . $staff->uid . '/edit');
    $this->assertText('Vacation mode');
    $this->assertText('Gone fishing rules');
  }

  public function testSettingsPageNeedsAdministerVacation() {
    $this->login($this->createStaff());
    $this->get('admin/rpg/vacation');
    $this->assertStatus(403);
  }

  // ---------------------------------------------------------------------------
  // Profile form.
  // ---------------------------------------------------------------------------

  public function testPlayersSeeNoVacationForm() {
    $player = $this->site->createUser(array('player'));
    $this->login($player);
    $this->get('user/' . $player->uid . '/edit');
    $this->assertStatus(200);
    $this->assertNoText('Vacation mode');
    $this->assertSame(0, $this->xpath()->query('//input[@value="Go on vacation"]')->length);
  }

  public function testStaffChooseADuration() {
    $staff = $this->createStaff();
    $this->login($staff);
    $this->get('user/' . $staff->uid . '/edit');
    $options = array();
    foreach ($this->xpath()->query('//select[@name="vacation_end"]/option') as $opt) {
      $options[$opt->getAttribute('value')] = trim($opt->textContent);
    }
    $this->assertSame(array(
      '3 days' => '3 days',
      '1 week' => '1 week',
      '2 weeks' => '2 weeks',
      '1 month' => '1 month',
      '2 months' => '2 months',
      '3 months' => '3 months',
      'indefinitely' => 'longer (resign)',
    ), $options);
  }

  public function testGoingOnVacationParksStaffRoles() {
    $staff = $this->createStaff();
    $this->goOnVacation($staff, '2 weeks');
    $this->assertStringContainsString('Your vacation notice has gone out', implode(' ', $this->messages('status')));
    $this->assertSame(array('player', 'vacation mode'), $this->rolesOf($staff));
  }

  // ---------------------------------------------------------------------------
  // Cron ends vacations.
  // ---------------------------------------------------------------------------

  public function testCronEndsVacationsEndingTodayAndRestoresRoles() {
    $staff = $this->createStaff();
    $before = $this->rolesOf($staff);
    $this->goOnVacation($staff);
    $this->endsToday($staff);
    $this->site->resetDailyCron('vacation_cron');

    $this->runCron();

    $this->assertSame($before, $this->rolesOf($staff));
  }

  public function testCronLeavesLaterVacationsAlone() {
    $staff = $this->createStaff();
    $this->goOnVacation($staff, '2 weeks');
    $this->site->resetDailyCron('vacation_cron');
    $this->runCron();
    $this->assertTrue($this->onVacation($staff));
  }

  public function testCronChecksOncePerDay() {
    $staff = $this->createStaff();
    $this->goOnVacation($staff);
    $this->endsToday($staff);
    $this->site->markDailyCronRan('vacation_cron', strtotime(date('m/d/Y') . ' 00:00:01'));
    $this->runCron();
    $this->assertTrue($this->onVacation($staff), 'Already checked today');
  }

  // ---------------------------------------------------------------------------
  // Known bugs: assert the intended behaviour, fail today.
  // ---------------------------------------------------------------------------

  /**
   * @group knownbug
   * The vacation fieldset only shows for users holding a role with "can go on
   * vacation", and for those users the form alter ends any current vacation as
   * soon as the page is built. Someone on vacation (player + vacation mode) gets
   * no form at all: no "Return from vacation", no end date.
   */
  public function testProfileShowsReturnButtonAndEndDateWhileOnVacation() {
    $staff = $this->createStaff();
    $this->goOnVacation($staff, '1 month');
    $this->get('user/' . $staff->uid . '/edit');
    $this->assertText('You are currently listed as on vacation');
    $this->assertText(date('m/d/Y', strtotime('+1 month')));
    $this->assertSame(1, $this->xpath()->query('//input[@value="Return from vacation"]')->length);
  }

  /**
   * @group knownbug
   * Same cause: "Return from vacation" can't be reached, so staff can't come
   * back early; only cron returns their roles.
   */
  public function testReturningFromVacationRestoresRoles() {
    $staff = $this->createStaff();
    $before = $this->rolesOf($staff);
    $this->goOnVacation($staff);
    $this->submitForm('user/' . $staff->uid . '/edit', array(), 'Return from vacation');
    $this->assertSame($before, $this->rolesOf($staff));
    $this->assertStringContainsString('Welcome back!', implode(' ', $this->messages('status')));
  }

  /**
   * @group knownbug
   * '#default_option' isn't a Form API key, so "3 days" is preselected instead
   * of the intended "1 week".
   */
  public function testDefaultDurationIsOneWeek() {
    $staff = $this->createStaff();
    $this->login($staff);
    $this->get('user/' . $staff->uid . '/edit');
    $selected = $this->xpath()->query('//select[@name="vacation_end"]/option[@selected]');
    $this->assertSame(1, $selected->length);
    $this->assertSame('1 week', $selected->item(0)->getAttribute('value'));
  }

  /**
   * @group knownbug
   * "longer (resign)" uses an undefined $account: the staff member keeps every
   * role and nothing is recorded.
   */
  public function testResignRemovesStaffRoles() {
    $staff = $this->createStaff();
    $this->goOnVacation($staff, 'indefinitely');
    $this->assertNotContains('storyteller', $this->rolesOf($staff));
  }

  /**
   * @group knownbug
   * _vacation_create_vacation() ends with array_merge($form, ...) on an undefined
   * $form: every "Go on vacation" prints a PHP warning.
   */
  public function testGoingOnVacationHasNoPhpErrors() {
    $this->goOnVacation($this->createStaff());
    $this->assertSame(array(), $this->pageErrorsFrom('vacation.module'));
  }

  /**
   * @group knownbug
   * Cron only ends vacations whose end falls inside today. If cron misses that
   * day, the vacation never ends and the roles never come back.
   */
  public function testCronEndsOverdueVacations() {
    $staff = $this->createStaff();
    $this->goOnVacation($staff);
    $this->site->setVacationEnd($staff, strtotime('-3 days'));
    $this->site->resetDailyCron('vacation_cron');
    $this->runCron();
    $this->assertContains('storyteller', $this->rolesOf($staff));
  }

}
