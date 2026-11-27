<?php

namespace Vaxia\Tests\Ui\PrivatemsgBlocks;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Production's private message blocks (D7 privatemsg 7.x-1.4):
 * - privatemsg-new, "New message indication": "You have a new message! Click
 *   here to read it." linking to the inbox, only while something is unread.
 * - privatemsg-menu, the links menu. privatemsg 2.x ships the equivalent as
 *   privatemsg_block ("All messages (N)" and "New message").
 *
 * The D7 test site has neither block placed the D11 way, so these run on the
 * port only.
 *
 * @group privatemsg_blocks
 */
class PrivatemsgBlocksTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('Runs against the D11 block plugins only.');
    }
    $this->site->placeBlock('privatemsg', 'privatemsg-new', 'content');
  }

  private function member() {
    return $this->site->createUser(array('player'), array('read privatemsg', 'write privatemsg'));
  }

  private function send($from, $to, $subject) {
    $this->login($from);
    $this->submitForm('messages/new', array('recipient' => $this->referenceValue($to->name, $to->uid, 'user'), 'subject' => $subject, 'body[value]' => 'Hello there'), 'Send message');
  }

  private function newLink() {
    return $this->xpath()->query('//a[@id="privatemsg-new-link"]');
  }

  public function testNothingWhenNoneUnread() {
    $this->login($this->member());
    $this->get('node');
    $this->assertSame(0, $this->newLink()->length);
    $this->assertNoPhpErrors();
  }

  public function testOneUnreadMessage() {
    $reader = $this->member();
    $this->send($this->member(), $reader, 'First');
    $this->login($reader);
    $this->get('node');
    $link = $this->newLink();
    $this->assertSame(1, $link->length);
    $this->assertSame('You have a new message! Click here to read it.', trim($link->item(0)->textContent));
    // It opens the thread itself (D7 linked the inbox; changed 2026-11-25).
    $this->assertMatchesRegularExpression('#/messages/view/\d+$#', $link->item(0)->getAttribute('href'));
    $this->assertNoPhpErrors();
  }

  public function testCountsUnreadThreads() {
    $reader = $this->member();
    $this->send($this->member(), $reader, 'One');
    $this->send($this->member(), $reader, 'Two');
    $this->login($reader);
    $this->get('node');
    $this->assertSame('You have 2 new messages! Click here to read them.', trim($this->newLink()->item(0)->textContent));
  }

  public function testLinkOpensTheOldestUnreadThread() {
    $reader = $this->member();
    $this->send($this->member(), $reader, 'Older news');
    sleep(1);
    $this->send($this->member(), $reader, 'Newer news');
    sleep(1);
    $this->login($reader);
    $this->get('node');
    $this->get($this->newLink()->item(0)->getAttribute('href'));
    $this->assertText('Older news');
    $this->assertNoText('Newer news');
    $this->get('node');
    $this->assertSame('You have a new message! Click here to read it.', trim($this->newLink()->item(0)->textContent));
  }

  public function testGoneOnceRead() {
    $reader = $this->member();
    $this->send($this->member(), $reader, 'Read me');
    // Unread means sent at or after the last view, to the second.
    sleep(1);
    $this->login($reader);
    $this->get('messages');
    $thread = $this->xpath()->query('//a[contains(@href, "/messages/view/")]');
    $this->assertGreaterThan(0, $thread->length, 'The thread in the inbox');
    $this->get($thread->item(0)->getAttribute('href'));
    $this->get('node');
    $this->assertSame(0, $this->newLink()->length);
  }

  public function testOnlyTheRecipientIsTold() {
    $sender = $this->member();
    $this->send($sender, $this->member(), 'Private');
    $this->login($sender);
    $this->get('node');
    $this->assertSame(0, $this->newLink()->length);
  }

  public function testNotForAnonymous() {
    $this->anonymous();
    $this->get('user/login');
    $this->assertSame(0, $this->newLink()->length);
    $this->assertNoPhpErrors();
  }

  public function testMenuBlockIsPrivatemsgsOwn() {
    $this->site->placeBlock('privatemsg', 'privatemsg_block', 'sidebar_first');
    $reader = $this->member();
    $this->login($reader);
    $this->get('node');
    $this->assertSame(1, $this->xpath()->query('//a[@href="/messages/new" and normalize-space(.)="New message"]')->length);
    $this->assertSame(1, $this->xpath()->query('//a[@href="/messages" and normalize-space(.)="All messages"]')->length);

    $this->send($this->member(), $reader, 'Counted');
    $this->login($reader);
    $this->get('node');
    $this->assertSame(1, $this->xpath()->query('//a[@href="/messages" and normalize-space(.)="All messages (1)"]')->length);

    $this->anonymous();
    $this->get('user/login');
    $this->assertNoRaw('privatemsg-block-new-messages-count');
  }

}
