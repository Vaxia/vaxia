<?php

namespace Vaxia\Tests\Ui\TextFormats;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Text formats as production ran them: chat posts turn short format tags
 * (*bold*, \italic\) into BBCode; wiki pages with several headings get a
 * table of contents (D7 simplewiki's autotoc).
 *
 * The D7 test site doesn't run production's formats (no flexifilter or
 * simplewiki), so these run on the port only.
 *
 * @group text_formats
 */
class TextFormatsTest extends VaxiaTestCase {

  protected function setUp(): void {
    parent::setUp();
    if (getenv('VAXIA_TARGET') !== 'd11') {
      $this->markTestSkipped('The D7 test site does not run production text formats.');
    }
  }

  private function postHtml($text) {
    $room = $this->site->createRoom();
    $player = $this->site->createUser(array('player'));
    $this->site->postComment($room, $player, $text);
    $this->login($player);
    $this->get('node/' . $room->nid);
    return $this->content;
  }

  public function testFullHtmlKeepsStyleBlocks() {
    // Full HTML (administrators only, unfiltered in D7) must not turn CSS
    // into visible text, as the footer block's <style> was.
    $page = $this->site->createNode('page', array(
      'title' => $this->site->uniqueName('Styled'),
      'body' => array(array('value' => "<style>\n.vaxia-styled{color:red;}\n.vaxia-styled p{margin:0;}\n</style>\n<p>Styled page</p>", 'format' => 'full_html')),
    ));
    $this->login($this->site->createUser(array('administrator')));
    $this->get('node/' . $page->nid);
    $this->assertRaw('.vaxia-styled{color:red;}');
    $this->assertNoRaw('&lt;style&gt;');
    $this->assertStringNotContainsString('.vaxia-styled{color:red;}', $this->pageText());
    // Line-break conversion must not put <br /> into the CSS (it breaks the
    // rules after it).
    $this->assertMatchesRegularExpression('#<style>[^<]*\.vaxia-styled p\{margin:0;\}[^<]*</style>#', $this->content);
  }

  public function testChatPostsStillEscapeScripts() {
    $html = $this->postHtml('hello <script>alert(1)</script>');
    $this->assertStringNotContainsString('<script>alert(1)</script>', $html);
  }

  public function testShortTagsMakeBoldAndItalic() {
    $html = $this->postHtml('I *really* \\mean\\ it');
    $this->assertMatchesRegularExpression('#<(strong|b)>really</\1>#', $html);
    $this->assertMatchesRegularExpression('#<(em|i)>mean</\1>#', $html);
  }

  public function testListItemsAreNotBold() {
    // One item per line, as production's posts and templates write them.
    $html = $this->postHtml("[list]\n[*]apples\n[*]pears\n[/list]");
    $this->assertMatchesRegularExpression('#<li>\s*apples\s*</li>#', $html);
    $this->assertMatchesRegularExpression('#<li>\s*pears\s*</li>#', $html);
  }

  // Production's BBCode (D7 bbcode 7.x-1.0): the HTML it rendered for each
  // tag production content uses.

  public function testHrStandsAlone() {
    // D7's [hr] took no closing tag.
    $html = $this->postHtml("above\n[hr]\nbelow");
    $this->assertMatchesRegularExpression('#above<br\s*/?>\s*<hr class="bb-hr"\s*/?>#', $html);
  }

  public function testAlignmentAndIndentBlocks() {
    $html = $this->postHtml('[center]Middle[/center][indent]Pushed[/indent][left]West[/left][right]East[/right]');
    $this->assertStringContainsString('<div style="text-align:center">Middle</div>', $html);
    $this->assertStringContainsString('<div style="padding-left: 20px;">Pushed</div>', $html);
    $this->assertStringContainsString('<div style="text-align:left">West</div>', $html);
    $this->assertStringContainsString('<div style="text-align:right">East</div>', $html);
  }

  public function testListsBulletedAndOrdered() {
    $html = $this->postHtml("[list]\n[*]apples\n[*]pears\n[/list]");
    $this->assertStringContainsString('<ul class="bb-list" style="list-style-type:circle;"><li>apples</li><li>pears</li></ul>', $html);
    $html = $this->postHtml("[list=1]\n[*]first\n[*]second\n[/list]\n[list=a]\n[*]alpha\n[/list]");
    $this->assertStringContainsString('<ol class="bb-list" style="list-style-type:decimal;"><li>first</li><li>second</li></ol>', $html);
    $this->assertStringContainsString('<ol class="bb-list" style="list-style-type:lower-alpha;"><li>alpha</li></ol>', $html);
    // No [*] left in the post body (the hidden subject and the format tips
    // keep the raw text).
    $body = '';
    foreach ($this->xpath()->query('//*[contains(@class, "field--name-comment-body")]') as $node) {
      $body .= $node->textContent;
    }
    $this->assertStringContainsString('alpha', $body);
    $this->assertStringNotContainsString('[*]', $body);
  }

  public function testNestedAndUnclosedLists() {
    // A nested list stays inside its item; D7 closed an unclosed [list] at
    // the end of the post.
    $html = $this->postHtml("[list]\n[*]outer\n[list]\n[*]inner\n[/list]\n[*]last");
    $this->assertStringContainsString('<li>outer<ul class="bb-list" style="list-style-type:circle;"><li>inner</li></ul></li><li>last</li></ul>', $html);
  }

  public function testTagsOpenedBeforeTheStarStayInTheItem() {
    // "[b][*]Name[/b]" (as some production posts wrote it) is a bold item,
    // not crossed tags left as text.
    $html = $this->postHtml("[list]\n[b][*]Sword[/b]\n[b][*]Shield[/b]\n[/list]");
    $this->assertMatchesRegularExpression('#<li><(strong|b)>Sword</\1></li><li><(strong|b)>Shield</\2></li>#', $html);
  }

  public function testImages() {
    $html = $this->postHtml('[img]http://example.com/pic.png[/img] [img=120x80]https://example.com/small.png[/img] [img align=right]/sites/default/files/x.png[/img]');
    $this->assertMatchesRegularExpression('#<img src="http://example\.com/pic\.png" alt(="")? class="bb-image"\s*/?>#', $html);
    $this->assertMatchesRegularExpression('#<img width="120" height="80" alt(="")? src="https://example\.com/small\.png" class="bb-image"\s*/?>#', $html);
    $this->assertMatchesRegularExpression('#<img src="/sites/default/files/x\.png" alt(="")? align="right" class="bb-image"\s*/?>#', $html);
  }

  public function testImagesRefuseScriptUrls() {
    // Not a URL D7's [img] accepted: left as text.
    $html = $this->postHtml('[img]javascript:alert(1)[/img] [img]http://example.com/a.png" onerror="alert(1)[/img]');
    $this->assertDoesNotMatchRegularExpression('#<img[^>]*(javascript|onerror)#i', $html);
    $this->assertStringContainsString('[img]javascript:alert(1)[/img]', $html);
  }

  public function testTables() {
    // Mostly wiki pages. The line breaks between rows and cells don't become
    // <br /> tags (D7 removed them).
    $html = $this->wikiHtml("[table]\n[tr]\n[td]North[/td]\n[td]South[/td]\n[/tr]\n[tr]\n[td]East[/td]\n[td]West[/td]\n[/tr]\n[/table]\nAfter");
    $this->assertStringContainsString('<table class="bb-table"><tr><td valign="top">North</td><td valign="top">South</td></tr><tr><td valign="top">East</td><td valign="top">West</td></tr></table>', $html);
  }

  public function testLinkForms() {
    $html = $this->postHtml('[url]http://example.com/page[/url] [url=https://example.com/]Example[/url] [url=www.example.com]Bare[/url] [url=javascript:alert(1)]Bad[/url]');
    $this->assertStringContainsString('<a href="http://example.com/page" class="bb-url" rel="nofollow">http://example.com/page</a>', $html);
    $this->assertStringContainsString('<a href="https://example.com/" class="bb-url" rel="nofollow">Example</a>', $html);
    // A www. link without a scheme gets http://.
    $this->assertStringContainsString('<a href="http://www.example.com" class="bb-url" rel="nofollow">Bare</a>', $html);
    $this->assertDoesNotMatchRegularExpression('#href="\s*javascript#i', $html);
  }

  public function testSizesArePixelsFromSixToFortyEight() {
    $html = $this->postHtml('[size=14]mid[/size] [size=100]huge[/size] [size=2]tiny[/size]');
    $this->assertStringContainsString('<span style="font-size:14px">mid</span>', $html);
    $this->assertStringContainsString('<span style="font-size:48px">huge</span>', $html);
    $this->assertStringContainsString('<span style="font-size:6px">tiny</span>', $html);
  }

  public function testQuotes() {
    $html = $this->postHtml('[quote=Bob]Hello[/quote] [quote]Anon[/quote]');
    $this->assertStringContainsString('<div class="bb-quote"><b>Bob wrote:</b><blockquote class="bb-quote-body">Hello</blockquote></div>', $html);
    $this->assertStringContainsString('<div class="bb-quote">Quote:<blockquote class="bb-quote-body">Anon</blockquote></div>', $html);
  }

  private function wikiHtml($body) {
    $page = $this->site->createNode('wiki_page', array(
      'title' => $this->site->uniqueName('Lore'),
      'body' => array(array('value' => $body, 'format' => 'wiki')),
    ));
    $this->login($this->site->createUser(array('player')));
    $this->get('node/' . $page->nid);
    return $this->content;
  }

  public function testWikiPagesWithThreeHeadingsGetContents() {
    $html = $this->wikiHtml('<h2>Geography</h2><p>Hills.</p><h2>History</h2><p>Old.</p><h3>Wars</h3><p>Many.</p>');
    $this->assertStringContainsString('class="wiki-toc"', $html);
    $this->assertMatchesRegularExpression('#<a href="\#toc-geography">Geography</a>#', $html);
    $this->assertMatchesRegularExpression('#<h2[^>]* id="toc-geography"#', $html);
  }

  public function testShortPagesGetNoContents() {
    $this->assertStringNotContainsString('class="wiki-toc"', $this->wikiHtml('<h2>Only</h2><p>One.</p>'));
  }

}
