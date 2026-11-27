<?php

namespace Drupal\vaxia_governance\Controller;

use Drupal\Core\Access\AccessResult;
use Drupal\Core\Controller\ControllerBase;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\Role;
use Drupal\webform\Entity\WebformSubmission;
use Symfony\Component\HttpFoundation\Request;

/**
 * Governance lists (D7 views anonymous_mailing_posts and leads).
 */
class GovernanceLists extends ControllerBase {

  /**
   * The anonymous mailer's "Posts made" tab: submissions older than two
   * weeks, newest first, 35 a page. Public, like D7's.
   */
  public function anonPosts(NodeInterface $node) {
    \Drupal::service('page_cache_kill_switch')->trigger();
    $formatter = \Drupal::service('date.formatter');
    $query = \Drupal::database()->select('webform_submission', 's')->fields('s', ['sid'])
      ->condition('s.entity_type', 'node')->condition('s.entity_id', $node->id())
      ->condition('s.created', strtotime('-2 weeks'), '<')
      ->orderBy('s.sid', 'DESC');
    $sids = $query->extend('Drupal\Core\Database\Query\PagerSelectExtender')->limit(35)->execute()->fetchCol();
    $rows = [];
    foreach (WebformSubmission::loadMultiple($sids) as $submission) {
      $rows[] = [
        $formatter->format($submission->getCreatedTime(), 'short'),
        $submission->getElementData('grievance_request') === 'no' ? $this->t('This is NOT a grievance complaint.') : $this->t('This is a grievance complaint.'),
        ['data' => ['#type' => 'processed_text', '#text' => (string) $submission->getElementData('message'), '#format' => 'wiki']],
      ];
    }
    return [
      'header' => [
        '#type' => 'processed_text',
        '#text' => 'The following submissions have been made to the anonymous mailer. Posts made within the last two weeks are not listed here in order to allow the site heads to handle and resolve any problems. Posts made to the mailer are required to be copied and pasted to the forums with the official reply from the heads - look to the forums for replies to any posts listed here.',
        '#format' => 'wiki',
      ],
      'table' => ['#type' => 'table', '#header' => [$this->t('Submitted'), $this->t('Grievance'), $this->t('Message')], '#rows' => $rows],
      'pager' => ['#type' => 'pager'],
      // Posts age into the list, so it can't be kept (anonymous visitors
      // too: the page cache ignores max-age).
      '#cache' => ['max-age' => 0],
    ];
  }

  /**
   * Only the mailer webform's node (the "Mailer Webform" setting) has the
   * tab.
   */
  public function anonPostsAccess(NodeInterface $node) {
    $config = $this->config('vaxia_governance.settings');
    return AccessResult::allowedIf((int) $node->id() === (int) $config->get('anon_mailer_webform'))->addCacheableDependency($config);
  }

  /**
   * Active leads, newest account first, 30 a page, with a name filter and the
   * last election underneath.
   */
  public function leads(Request $request) {
    $storage = $this->entityTypeManager()->getStorage('user');
    $formatter = \Drupal::service('date.formatter');
    $query = $storage->getQuery()->accessCheck(FALSE)->condition('status', 1)->condition('roles', 'lead')->sort('created', 'DESC')->sort('uid', 'DESC')->pager(30);
    $name = trim((string) $request->query->get('name'));
    if ($name !== '') {
      $query->condition('name', $name, 'CONTAINS');
    }
    $ago = fn($time) => $time ? $this->t('@time ago', ['@time' => $formatter->formatTimeDiffSince($time)]) : '';
    $rows = [];
    foreach ($storage->loadMultiple($query->execute()) as $account) {
      $badges = array_map(fn($r) => $r->label(), array_filter(Role::loadMultiple($account->getRoles(TRUE)), fn($r) => !in_array($r->id(), ['player', 'new_account'], TRUE)));
      $rows[] = [$account->toLink(), $ago($account->getCreatedTime()), $ago($account->getLastLoginTime()), implode(', ', $badges)];
    }
    $build = [
      'filter' => [
        '#type' => 'html_tag', '#tag' => 'form', '#attributes' => ['method' => 'get'],
        'name' => ['#type' => 'textfield', '#title' => $this->t('Name'), '#name' => 'name', '#value' => $name],
        'submit' => ['#type' => 'html_tag', '#tag' => 'button', '#value' => $this->t('Apply'), '#attributes' => ['type' => 'submit']],
      ],
      'table' => ['#type' => 'table', '#header' => [$this->t('Name'), $this->t('Created date'), $this->t('Last login'), $this->t('Badges')], '#rows' => $rows],
      'pager' => ['#type' => 'pager'],
      // "Time ago" ages, so the page isn't kept.
      '#cache' => ['max-age' => 0],
    ];
    // Footer: D7 embedded the proposals "Elections" display, which only
    // players could see: the newest published election, rendered in full.
    if (in_array('player', $this->currentUser()->getRoles(), TRUE)) {
      $nodes = $this->entityTypeManager()->getStorage('node');
      $ids = $nodes->getQuery()->accessCheck(TRUE)->condition('type', 'proposal')->condition('status', 1)->condition('field_proposal_type', 'election')->sort('created', 'DESC')->range(0, 1)->execute();
      if ($ids) {
        $election = $nodes->load(reset($ids));
        $builder = $this->entityTypeManager()->getViewBuilder('node');
        // Built now so the comments (D7 showed them only on the node's own
        // page) can be dropped; D7 titled the teaser-like full view.
        $view = $builder->build($builder->view($election, 'full'));
        foreach ($election->getFieldDefinitions() as $field => $definition) {
          if ($definition->getType() === 'comment') {
            unset($view[$field]);
          }
        }
        $build['election'] = [
          'title' => ['#type' => 'html_tag', '#tag' => 'h2', 'link' => $election->toLink()->toRenderable()],
          'node' => $view,
        ];
      }
    }
    return $build;
  }

  /**
   * Organization goals: the editable table, then "Add a new Organization"
   * (D7's footer embedded the node add form).
   */
  public function organizationGoals() {
    $build['goals'] = $this->formBuilder()->getForm('\Drupal\vaxia_governance\Form\OrganizationGoalsForm');
    $build['add'] = [
      '#prefix' => '<hr><h4>' . $this->t('Add a new Organization') . '</h4><p>',
      '#suffix' => '</p>',
      'form' => $this->entityFormBuilder()->getForm($this->entityTypeManager()->getStorage('node')->create(['type' => 'organization_goal'])),
    ];
    return $build;
  }

}
