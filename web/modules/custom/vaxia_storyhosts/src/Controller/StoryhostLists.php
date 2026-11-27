<?php

namespace Drupal\vaxia_storyhosts\Controller;

use Drupal\Core\Controller\ControllerBase;
use Symfony\Component\HttpFoundation\Request;

/**
 * Staff lists (D7 storyhosts and storyhost_duties views).
 */
class StoryhostLists extends ControllerBase {

  private function table(array $header, array $rows, string $empty): array {
    return [
      'table' => ['#type' => 'table', '#header' => $header, '#rows' => $rows, '#empty' => $empty],
      'pager' => ['#type' => 'pager'],
      '#cache' => ['max-age' => 0],
    ];
  }

  /**
   * Characters with goals.
   */
  public function goals(Request $request) {
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'character_sheet')->condition('status', 1)->exists('field_character_goals')->sort('title')->pager(50);
    $rows = [];
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      $rows[] = [$node->toLink(), $node->getOwner()->getDisplayName(), ['data' => ['#markup' => check_markup($node->get('field_character_goals')->value, $node->get('field_character_goals')->format)]]];
    }
    return $this->table([$this->t('Character'), $this->t('Player'), $this->t('Goals')], $rows, $this->t('No character goals.'));
  }

  /**
   * Every skill on a published character.
   */
  public function skills(Request $request) {
    $storage = $this->entityTypeManager()->getStorage('node');
    $query = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'character_sheet')->condition('status', 1)->sort('title')->pager(50);
    $name = trim((string) $request->query->get('skill'));
    $rows = [];
    foreach ($storage->loadMultiple($query->execute()) as $node) {
      if (!$node->hasField('field_skill')) {
        continue;
      }
      foreach ($node->get('field_skill')->referencedEntities() as $skill) {
        $skill_name = (string) $skill->get('field_skill_name')->value;
        if ($name !== '' && stripos($skill_name, $name) === FALSE) {
          continue;
        }
        $realm = $node->get('field_tag_realm')->entity;
        $rows[] = [$node->toLink(), $node->getOwner()->getDisplayName(), $realm ? $realm->label() : '', $skill_name, (string) $skill->get('field_skill_desc')->value];
      }
    }
    return $this->table([$this->t('Character'), $this->t('Player'), $this->t('Realm'), $this->t('Skill'), $this->t('Description')], $rows, $this->t('No skills.'));
  }

  /**
   * Storyhost activity over the last year (D7 user_activity_totals).
   */
  public function activity() {
    $storage = $this->entityTypeManager()->getStorage('user');
    $ids = $storage->getQuery()->accessCheck(FALSE)->condition('status', 1)->condition('roles', 'storyteller')->exists('field_storyhost_character')->sort('created', 'DESC')->execute();
    $rows = [];
    foreach ($storage->loadMultiple($ids) as $account) {
      $character = $account->get('field_storyhost_character')->entity;
      $uid = (int) $account->id();
      $rows[] = [
        $uid,
        $account->toLink(),
        $character ? (string) $character->get('field_titles')->value : '',
        $character ? $character->toLink() : '',
        vaxia_storyhosts_counted_sessions($uid),
        vaxia_storyhosts_prompt_evaluations($uid, 'character_sheet', 'character_evaluation_approved'),
        vaxia_storyhosts_prompt_evaluations($uid, 'items', 'item_evaluation_approved'),
        vaxia_storyhosts_prompt_evaluations($uid, 'scene', 'scene_evaluation_ruled'),
        vaxia_storyhosts_created_count($uid, 'wiki_page'),
        vaxia_storyhosts_revised_count($uid, 'wiki_page'),
        vaxia_storyhosts_sh_forum_replies($uid),
        vaxia_storyhosts_oldest_unread($uid),
        vaxia_storyhosts_unread_threads($uid),
        $account->getLastAccessedTime() ? \Drupal::service('date.formatter')->format($account->getLastAccessedTime(), 'short') : $this->t('Never'),
      ];
    }
    $build = $this->table([
      $this->t('Uid'), $this->t('Name'), $this->t('Titles'), $this->t('Title'), $this->t('Ran Sessions'), $this->t('Characters Eval'),
      $this->t('Items Eval'), $this->t('Scenes Eval'), $this->t('Created Wiki'), $this->t('Revised Wiki'), $this->t('Replies on SH Requests'),
      $this->t('Oldest Unread PM'), $this->t('Count Unread PM Threads'), $this->t('Last access'),
    ], $rows, $this->t('No storyhosts.'));
    $build['footer'] = ['#markup' => '<p>' . $this->t('Values are displayed for full storyhosts and those on vacation. They are calculated based on all activity within the last year. Items and characters must have been promptly evaluated. Sessions must have qualified under session requirements.') . '</p>', '#weight' => 10];
    return $build;
  }

}
