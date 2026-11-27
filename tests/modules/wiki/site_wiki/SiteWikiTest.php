<?php

namespace Vaxia\Tests\Wiki\SiteWiki;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Site wiki: wiki pages in the "wiki" text format automatically link the first
 * mention of any other wiki page's title. Edits need a revision note. Locked
 * pages (Wiki workflow) can only be edited with "edit locked site wiki". A
 * disambiguation block lists other pages with the same title, and admins keep
 * page templates that a block offers on the edit form.
 *
 * @group site_wiki
 */
class SiteWikiTest extends VaxiaTestCase {

  private function wiki($title, $body = 'Text.', array $values = array()) {
    return $this->site->createNode('wiki_page', $values + array(
      'title' => $title,
      'body' => array(array('value' => $body, 'format' => 'wiki')),
      // Required on the edit form.
      'field_organization_type' => $this->site->term('organization_type', 'Guild'),
      'field_tag_department' => $this->site->term('department', 'Setting'),
      'field_tag_location' => $this->site->term('location_depth', 'City'),
      'field_tag_type' => $this->site->term('setting_type', 'Location'),
      // Required too; D7 defaulted it to its Storyhost realm term.
      'field_tag_realm' => $this->site->term('realm', 'Vaxia'),
    ));
  }

  private function editor(array $permissions = array()) {
    return $this->site->createUser(array('player'), array_merge(array('edit any wiki_page content', 'access content'), $permissions));
  }

  private function bodyHtml($node) {
    $this->get('node/' . $node->nid);
    // D7 classes fields field-name-body, D11 field--name-body.
    $body = $this->xpath()->query('//div[contains(@class, "field-name-body") or contains(@class, "field--name-body")]')->item(0);
    $this->assertNotNull($body, 'Body shown');
    return $body->ownerDocument->saveHTML($body);
  }

  // ---------------------------------------------------------------------------
  // Auto links.
  // ---------------------------------------------------------------------------

  public function testOtherWikiTitlesAreLinked() {
    $place = $this->wiki($this->site->uniqueName('Rivermoot'));
    $page = $this->wiki($this->site->uniqueName('Travel'), 'We sail to ' . $place->title . ' today.');
    $this->anonymous();
    $this->assertMatchesRegularExpression('#<a href="/node/' . $place->nid . '"[^>]*>' . $place->title . '</a>#', $this->bodyHtml($page));
  }

  public function testOnlyTheFirstMentionIsLinked() {
    $place = $this->wiki($this->site->uniqueName('Rivermoot'));
    $page = $this->wiki($this->site->uniqueName('Travel'), 'From ' . $place->title . ' we go back to ' . $place->title . ' again.');
    $this->anonymous();
    $this->assertSame(1, substr_count($this->bodyHtml($page), 'href="/node/' . $place->nid . '"'));
  }

  public function testUnpublishedPagesAreNotLinked() {
    $place = $this->wiki($this->site->uniqueName('Rivermoot'), 'Text.', array('status' => 0));
    $page = $this->wiki($this->site->uniqueName('Travel'), 'We sail to ' . $place->title . ' today.');
    $this->anonymous();
    $this->assertStringNotContainsString('href="/node/' . $place->nid . '"', $this->bodyHtml($page));
  }

  /**
   * @group knownbug
   * The link replaces the whole match, including the character after the
   * title: "to Rivermoot." loses its period ("to Rivermoot ").
   */
  public function testPunctuationAfterALinkIsKept() {
    $place = $this->wiki($this->site->uniqueName('Rivermoot'));
    $page = $this->wiki($this->site->uniqueName('Travel'), 'We sail to ' . $place->title . '. Then home.');
    $this->anonymous();
    $this->assertStringContainsString($place->title . '</a>.', $this->bodyHtml($page));
  }

  // ---------------------------------------------------------------------------
  // Editing.
  // ---------------------------------------------------------------------------

  public function testEditsNeedARevisionNote() {
    $page = $this->wiki($this->site->uniqueName('Lore'));
    $this->login($this->editor());
    $this->submitForm('node/' . $page->nid . '/edit', array('log' => ''), 'Save');
    $this->assertStringContainsString('field is required', implode(' ', $this->messages('error')));
  }

  public function testRevisionNoteRequirementCanBeTurnedOff() {
    $this->inSession($this->superuser(), function () {
      $this->submitForm('admin/config/system/site_wiki', array('site_wiki_revision' => FALSE), 'Save configuration');
    });
    $page = $this->wiki($this->site->uniqueName('Lore'));
    $this->login($this->editor());
    $this->submitForm('node/' . $page->nid . '/edit', array('log' => ''), 'Save');
    $errors = $this->messages('error');
    $this->inSession($this->superuser(), function () {
      $this->submitForm('admin/config/system/site_wiki', array('site_wiki_revision' => TRUE), 'Save configuration');
    });
    $this->assertStringNotContainsString('field is required', implode(' ', $errors));
  }

  public function testLockedPagesCannotBeEdited() {
    $page = $this->wiki($this->site->uniqueName('Canon'));
    $this->transition($page, 'locked');
    $this->login($this->editor());
    $this->get('node/' . $page->nid . '/edit');
    $this->assertContains('This wiki article is locked. You are not allowed to edit it.', $this->messages());
    $this->assertSame(0, $this->xpath()->query('//input[@value="Save"]')->length);
  }

  public function testLockedPagesCanBeEditedWithPermission() {
    $page = $this->wiki($this->site->uniqueName('Canon'));
    $this->transition($page, 'locked');
    $this->login($this->editor(array('edit locked site wiki')));
    $this->submitForm('node/' . $page->nid . '/edit', array('log' => 'Fix typo', 'body[und][0][value]' => 'Corrected.'), 'Save');
    $this->get('node/' . $page->nid);
    $this->assertText('Corrected.');
  }

  public function testOpenPagesCanBeEdited() {
    $page = $this->wiki($this->site->uniqueName('Draft'));
    $this->transition($page, 'open');
    $this->login($this->editor());
    $this->get('node/' . $page->nid . '/edit');
    $this->assertSame(1, $this->xpath()->query('//input[@value="Save"]')->length);
  }

  // ---------------------------------------------------------------------------
  // Blocks and admin pages.
  // ---------------------------------------------------------------------------

  public function testDisambiguationListsPagesWithTheSameTitle() {
    $this->site->placeBlock('site_wiki', 'site_wiki_disamb', 'content');
    $title = $this->site->uniqueName('Mercury');
    $planet = $this->wiki($title, 'A planet near the sun.');
    $god = $this->wiki($title, 'A messenger god.');
    $this->anonymous();
    $this->get('node/' . $planet->nid);
    $this->assertText('Other wiki articles share the same title...');
    $this->assertText('A messenger god.');
    $this->assertSame(1, $this->xpath()->query('//a[@href="/node/' . $god->nid . '"]')->length);
  }

  public function testTemplatesAreOfferedOnTheEditForm() {
    $this->site->placeBlock('site_wiki', 'site_wiki_templates', 'content');
    $this->login($this->site->createUser(array(), array('administer site wiki')));
    $this->get('admin/config/system/site_wiki');
    // The empty row at the end adds a template.
    $blank = $this->xpath()->query('//input[contains(@name, "[template_name]") and not(@value)] | //input[contains(@name, "[template_name]") and @value=""]')->item(0);
    $this->assertNotNull($blank, 'Blank template row');
    $prefix = substr($blank->getAttribute('name'), 0, -strlen('[template_name]'));
    $this->submitForm(NULL, array(
      $prefix . '[template_name]' => 'Location',
      $prefix . '[template]' => '== Geography ==',
    ), 'Save configuration');

    $this->login($this->editor(array('create wiki_page content')));
    $this->get('node/add/wiki-page');
    $this->assertText('Location');
    $this->assertRaw('== Geography ==');
    $this->assertRaw('/js/site_wiki.js');
  }

  public function testSettingsNeedPermission() {
    $this->login($this->editor());
    $this->get('admin/config/system/site_wiki');
    $this->assertStatus(403);
  }

  public function testWikiContentListNeedsAdministerNodes() {
    $this->login($this->editor());
    $this->get('admin/content/wiki-content');
    $this->assertStatus(403);
    $this->login($this->site->createUser(array(), array('administer nodes', 'access administration pages')));
    $this->get('admin/content/wiki-content');
    $this->assertStatus(200);
  }

}
