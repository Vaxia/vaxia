<?php

namespace Vaxia\Tests\Administration\AnonMailer;

use Vaxia\Tests\VaxiaTestCase;

/**
 * The anonymous mailer's "Posts made" tab, node/9368/anon-posts (D7 view
 * anonymous_mailing_posts): public; submissions older than two weeks, newest
 * first, 35 a page, with whether each is a grievance and its message. Only
 * the mailer's node has the tab.
 *
 * The D7 test site doesn't have production's views, so these run on the port
 * only.
 *
 * @group anon_mailer
 * @group views_pages
 */
class AnonPostsPageTest extends VaxiaTestCase {

  const MAILER = 9368;

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not have production\'s views.');
    }
    $this->site->createWebformAt(self::MAILER, 'Anonymous mailer', array(1, 2), array(
      1 => array('cid' => 1, 'pid' => 0, 'form_key' => 'message', 'name' => 'Your message', 'type' => 'textarea', 'value' => '', 'extra' => array(), 'mandatory' => 0, 'weight' => 0),
      5 => array('cid' => 5, 'pid' => 0, 'form_key' => 'grievance_request', 'name' => 'Is this a grievance request?', 'type' => 'select', 'value' => 'no',
        'extra' => array('items' => "no|This is NOT a grievance request.\nyes|This is a grievance request.", 'multiple' => 0, 'aslist' => 1), 'mandatory' => 0, 'weight' => 2),
    ));
    // Earlier tests' submissions out of the way.
    $this->site->backdateWebformSubmissions(self::MAILER, strtotime('-5 years'));
  }

  private function send($message, $grievance = 'no') {
    $this->anonymous();
    $this->submitForm('node/' . self::MAILER, array('submitted[message]' => $message, 'submitted[grievance_request]' => $grievance), 'Submit');
  }

  /**
   * Message column texts, in page order.
   */
  private function messagesListed() {
    $texts = array();
    foreach ($this->xpath()->query('//table//tbody/tr/td[3]') as $td) {
      $texts[] = trim($td->textContent);
    }
    return $texts;
  }

  public function testOldPostsArePublic() {
    $this->send('Please fix the tavern.');
    $this->site->backdateWebformSubmissions(self::MAILER, strtotime('-15 days'));
    $this->send('Too fresh to list.');

    $this->anonymous();
    $this->get('node/' . self::MAILER . '/anon-posts');
    $this->assertStatus(200);
    $this->assertText('Anonymous mailing posts');
    $this->assertText('Posts made within the last two weeks are not listed here');
    $this->assertText('Please fix the tavern.');
    $this->assertNoText('Too fresh to list.');
    $this->assertText('This is NOT a grievance complaint.');
    $this->assertSame(array('Submitted', 'Grievance', 'Message'), array_map(function ($th) {
      return trim($th->textContent);
    }, iterator_to_array($this->xpath()->query('//table//thead//th'))));
  }

  public function testGrievancesAndNewestFirst() {
    $this->send('first post');
    $this->send('second post', 'yes');
    $this->site->backdateWebformSubmissions(self::MAILER, strtotime('-20 days'));
    $this->anonymous();
    $this->get('node/' . self::MAILER . '/anon-posts');
    $listed = $this->messagesListed();
    $this->assertSame(array('second post', 'first post'), array_slice($listed, 0, 2));
    $this->assertSame('This is a grievance complaint.', trim($this->xpath()->query('//table//tbody/tr[1]/td[2]')->item(0)->textContent));
  }

  public function testThirtyFiveAPage() {
    for ($i = 1; $i <= 36; $i++) {
      $this->send('note ' . $i);
    }
    $this->site->backdateWebformSubmissions(self::MAILER, strtotime('-20 days'));
    $this->anonymous();
    $this->get('node/' . self::MAILER . '/anon-posts');
    $listed = $this->messagesListed();
    $this->assertCount(35, $listed);
    $this->assertSame('note 36', $listed[0]);
    $this->get('node/' . self::MAILER . '/anon-posts', array('page' => 1));
    $this->assertSame('note 1', $this->messagesListed()[0]);
  }

  public function testTabOnlyOnTheMailer() {
    $this->anonymous();
    $this->get('node/' . self::MAILER);
    $this->assertRaw('/node/' . self::MAILER . '/anon-posts');
    $this->assertText('Posts made');
    $other = $this->site->createNode('page');
    $this->get('node/' . $other->nid . '/anon-posts');
    $this->assertStatus(403);
  }

}
