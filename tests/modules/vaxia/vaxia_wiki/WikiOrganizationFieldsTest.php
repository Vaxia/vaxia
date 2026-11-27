<?php

namespace Vaxia\Tests\Vaxia\VaxiaWiki;

use Vaxia\Tests\VaxiaTestCase;

/**
 * Wiki pages show their organization fields (goals, membership, strength,
 * influence, organization type, reputation) only when the page is about an
 * organization: its type is "Organization" (D7 conditional_fields 34, 67-72).
 *
 * @group vaxia_wiki
 */
class WikiOrganizationFieldsTest extends VaxiaTestCase {

  private function wikiPage(array $values = array()) {
    return $this->site->createNode('wiki_page', $values + array(
      'title' => $this->site->uniqueName('Page'),
      'field_org_membership_size' => 3,
      'field_org_economic_influence' => 2,
    ));
  }

  public function testOrdinaryPagesHideTheOrganizationFields() {
    $page = $this->wikiPage();
    $this->login($this->superuser());
    $this->get('node/' . $page->nid);
    $this->assertStatus(200);
    $this->assertNoText('Membership Size');
    $this->assertNoText('Economic Influence');
  }

  public function testOrganizationPagesShowThem() {
    $page = $this->wikiPage(array('field_tag_type' => $this->site->term('setting_type', 'Organization')));
    $this->login($this->superuser());
    $this->get('node/' . $page->nid);
    $this->assertText('Membership Size');
    $this->assertText('Economic Influence');
  }

}
