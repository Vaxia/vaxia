<?php

/**
 * @file
 * Scenes (D7 scene_record and vaxia_scene): a scene collects chat posts for
 * ruling and shows them as a results table.
 */

use Drupal\comment\CommentInterface;
use Drupal\Core\Form\FormStateInterface;
use Drupal\Core\Link;
use Drupal\Core\Render\Markup;
use Drupal\Core\Url;
use Drupal\node\NodeInterface;
use Drupal\user\Entity\User;
use Drupal\vaxia_dice\Render;

/**
 * The scene's posts, oldest first.
 */
function vaxia_storyhosts_scene_posts(NodeInterface $scene): array {
  $posts = [];
  foreach ($scene->get('field_rp_posts')->referencedEntities() as $comment) {
    $posts[(int) $comment->id()] = $comment;
  }
  ksort($posts);
  return $posts;
}

/**
 * The character a post was made as, if any.
 */
function vaxia_storyhosts_post_character(CommentInterface $comment): ?NodeInterface {
  return $comment->hasField('field_comment_character') ? $comment->get('field_comment_character')->entity : NULL;
}

/**
 * Whether the current user may see a scene: staff with "view all scenes", its
 * author with "view own scenes", or a player with a character in it.
 */
function vaxia_storyhosts_scene_viewable(NodeInterface $scene): bool {
  $account = \Drupal::currentUser();
  if ($account->hasPermission('view all scenes')) {
    return TRUE;
  }
  if ((int) $scene->getOwnerId() === (int) $account->id() && $account->hasPermission('view own scenes')) {
    return TRUE;
  }
  foreach (vaxia_storyhosts_scene_posts($scene) as $comment) {
    $character = vaxia_storyhosts_post_character($comment);
    if ($character && (int) $character->getOwnerId() === (int) $account->id()) {
      return TRUE;
    }
  }
  return FALSE;
}

/**
 * Implements hook_ENTITY_TYPE_presave() for nodes: scene posts kept in post
 * order.
 */
function vaxia_storyhosts_scene_presave(NodeInterface $node): void {
  if ($node->bundle() !== 'scene') {
    return;
  }
  $refs = [];
  foreach ($node->get('field_rp_posts')->getValue() as $item) {
    if (!empty($item['target_id'])) {
      $refs[(int) $item['target_id']] = $item;
    }
  }
  ksort($refs);
  $node->set('field_rp_posts', array_values($refs));
}

/**
 * Implements hook_ENTITY_TYPE_view() for nodes: the results table replaces the
 * post list.
 */
function vaxia_storyhosts_scene_view(array &$build, NodeInterface $node, string $view_mode): void {
  if ($node->bundle() !== 'scene' || $view_mode !== 'full' || !vaxia_storyhosts_scene_viewable($node)) {
    return;
  }
  $posts = vaxia_storyhosts_scene_posts($node);
  if ($posts) {
    $build['field_rp_posts'] = vaxia_storyhosts_scene_table($node, $posts) + ['#weight' => $build['field_rp_posts']['#weight'] ?? 100];
  }
  $build['#cache']['max-age'] = 0;
}

/**
 * Who is posting: the character, or the player for posts made without one.
 */
function vaxia_storyhosts_post_by(CommentInterface $comment, ?NodeInterface $character): string {
  return $character ? $character->label() : (string) $comment->getOwner()->getDisplayName();
}

/**
 * The scene results table (D7 _scene_record_view_table()).
 */
function vaxia_storyhosts_scene_table(NodeInterface $scene, array $posts): array {
  $results = vaxia_storyhosts_scene_results($scene, $posts);
  $roller = \Drupal::service('vaxia_dice.roller');
  $rows = [];
  foreach ($posts as $cid => $comment) {
    $text = check_markup($comment->get('comment_body')->value, $comment->get('comment_body')->format);
    if ($comment->hasField('field_ooc') && !$comment->get('field_ooc')->isEmpty()) {
      $text .= 'OOC: ' . check_markup($comment->get('field_ooc')->value, $comment->get('field_ooc')->format);
    }
    $rolls = $roller->rolls($cid, 'c');
    if ($rolls) {
      $text .= Render::markup($rolls);
    }
    $rows[] = [
      ['data' => ['#markup' => $results[$cid]['character']]],
      ['data' => ['#markup' => Markup::create($text)]],
      ['data' => ['#theme' => 'item_list', '#items' => array_map(fn($m) => ['#markup' => $m], $results[$cid]['messages'])], 'nowrap' => 'nowrap'],
    ];
  }
  // D7 passed 'headers' to theme('table'), so the headings never showed.
  return [
    '#type' => 'table',
    '#header' => [t('Character'), t('Scene posts'), t('Results')],
    '#rows' => $rows,
  ];
}

/**
 * Per-post results: dice outcome, running totals, and each goal's helpers
 * (hook_vaxia_scene_goal_info(), D7 hook_scene_record_goal_info()).
 */
function vaxia_storyhosts_scene_results(NodeInterface $scene, array $posts): array {
  $goals = array_column($scene->get('field_goal')->getValue(), 'value') ?: ['misc'];
  $callbacks = [];
  foreach (\Drupal::moduleHandler()->invokeAll('vaxia_scene_goal_info') as $callback => $info) {
    $types = (array) $info['type'];
    if (in_array('all', $types, TRUE) || array_intersect($goals, $types)) {
      $callbacks[] = $callback;
    }
  }
  $difficulty = (int) ($scene->get('field_difficulty')->value ?: 25);
  $bonus = $scene->get('field_rp_bonus')->value ? 10 : 0;
  $context = ['diff' => $difficulty, 'bonus' => $bonus, 'ongoing' => ['_ruling' => ['PPs' => 0, '1s' => 0, '100s' => 0, 'crit_fails' => 0, 'crit_wins' => 0]]];
  $roller = \Drupal::service('vaxia_dice.roller');
  $results = [];
  foreach ($posts as $cid => $comment) {
    $messages = [];
    $context['roll_set'] = _dice_ruler_read_dice($roller->rolls($cid, 'c'), $difficulty, $bonus);
    $context['character'] = vaxia_storyhosts_post_character($comment);
    $context['comment'] = $comment;
    $by = vaxia_storyhosts_post_by($comment, $context['character']);
    $character_cell = '<b>' . ($context['character'] ? Link::fromTextAndUrl($by, $context['character']->toUrl())->toString() : htmlspecialchars($by)) . '</b>';
    foreach (['PPs', '1s', '100s', 'crit_fails', 'crit_wins'] as $key) {
      $context['ongoing']['_ruling'][$key] += $context['roll_set'][$key];
    }
    $ruling = $context['ongoing']['_ruling'];
    $messages[] = '<b>' . Link::fromTextAndUrl(t('Link to post'), $comment->toUrl())->toString() . '</b>';
    if (!empty($context['roll_set']['mights'])) {
      $won = $context['roll_set']['success'];
      $messages[] = '<b>' . t('Diff @difficulty / RP bonus @rp_bonus', ['@difficulty' => $difficulty, '@rp_bonus' => $bonus]) . '</b>';
      $messages[] = '<b>' . t('Roll @by_verb by @bys, @pp_verb @pps PP.', [
        '@by_verb' => $won ? 'wins' : 'fails',
        '@bys' => number_format((float) $context['roll_set']['by']),
        '@pp_verb' => $won ? 'gained' : 'lost',
        '@pps' => number_format((float) $context['roll_set']['PPs']),
      ]) . '</b>';
      $messages[] = '<b><span style="color:green;">' . $ruling['crit_wins'] . ' crit wins-by</span> / <span style="color:red;">' . $ruling['crit_fails'] . ' crit fails-by</span></b>';
      $messages[] = '<b>' . number_format((float) $ruling['PPs']) . ' PP / <span style="color:green;">' . $ruling['100s'] . ' 100s</span> / <span style="color:red;">' . $ruling['1s'] . ' 1s</span></b>';
    }
    else {
      $messages[] = '<b>' . t('No roll found to evaluate.') . ' <span style="color:red;">' . t('Skipped!') . '</span></b>';
    }
    foreach ($callbacks as $callback) {
      $messages = array_merge($messages, (array) $callback($comment, $context));
    }
    $results[$cid] = ['character' => $character_cell, 'messages' => $messages];
  }
  return $results;
}

/**
 * Implements hook_vaxia_scene_goal_info(): running KXP/XP totals on every
 * scene.
 */
function vaxia_storyhosts_vaxia_scene_goal_info() {
  return ['vaxia_storyhosts_scene_kxp' => ['label' => 'A running total of all results.', 'type' => ['all']]];
}

/**
 * KXP in each post and running KXP/XP per character (D7
 * _scene_record_calculate_results_all()).
 */
function vaxia_storyhosts_scene_kxp(CommentInterface $comment, array &$context): array {
  $messages = [];
  $ratio = (float) \Drupal::config('vaxia_core.settings')->get('vaxia_kxp_ratio') ?: 70;
  $context['ongoing']['scene_record']['all'] = $context['ongoing']['scene_record']['all'] ?? [];
  $totals = &$context['ongoing']['scene_record']['all'];
  // D7 broke on posts without a character; they count for no one.
  if ($context['character']) {
    $name = $context['character']->label();
    $totals[$name] = $totals[$name] ?? 0;
    $kxp = \Drupal::service('vaxia_core.xp')->kxpFor((string) $comment->get('comment_body')->value);
    $messages[] = t('@kxp KXP in this post.', ['@kxp' => $kxp]);
    $totals[$name] += $kxp;
  }
  $total_kxp = $total_xp = 0;
  foreach ($totals as $name => $kxp) {
    $messages[] = t('@kxp KXP for %name.', ['%name' => $name, '@kxp' => $kxp]);
    $xp = $kxp >= $ratio ? floor($kxp / $ratio) : 0;
    $messages[] = t('@xp XP for %name', ['%name' => $name, '@xp' => $xp]);
    $total_kxp += $kxp;
    $total_xp += $xp;
  }
  if (count($totals) > 1) {
    $messages[] = t('Total @total_kxp KXP for all.', ['@total_kxp' => $total_kxp]);
    $messages[] = t('Total @total_xp XP for all.', ['@total_xp' => $total_xp]);
  }
  return $messages;
}

/**
 * Implements hook_form_BASE_FORM_ID_alter() for node forms: the scene
 * assistant on the scene form, and new scenes go to their Workflow tab.
 */
function vaxia_storyhosts_form_node_form_alter(array &$form, FormStateInterface $form_state) {
  $node = $form_state->getFormObject()->getEntity();
  if ($node->bundle() !== 'scene') {
    return;
  }
  $posts = $node->isNew() ? [] : vaxia_storyhosts_scene_posts($node);
  $refresh = [
    '#type' => 'button',
    '#value' => t('Refresh scene'),
    '#ajax' => ['callback' => 'vaxia_storyhosts_scene_form_ajax', 'wrapper' => 'scene-record-form-display'],
  ];
  $form['scene_record_assist'] = [
    '#type' => 'details',
    '#title' => t('Scene assistant'),
    '#open' => FALSE,
    '#weight' => isset($form['field_rp_posts']['#weight']) ? $form['field_rp_posts']['#weight'] - 0.5 : 100,
    'scene_record_refresh' => $refresh + ['#name' => 'scene_record_refresh'],
    'scene_record_display' => [
      '#prefix' => '<div id="scene-record-form-display">',
      '#suffix' => '</div>',
      'table' => $posts ? vaxia_storyhosts_scene_table($node, $posts) : [],
    ],
    'scene_record_refresh2' => $refresh + ['#name' => 'scene_record_refresh2'],
  ];
  if ($node->isNew()) {
    $form['actions']['submit']['#submit'][] = 'vaxia_storyhosts_scene_form_submit';
  }
}

function vaxia_storyhosts_scene_form_ajax(array $form, FormStateInterface $form_state) {
  return $form['scene_record_assist']['scene_record_display'];
}

/**
 * New scenes go to their Workflow tab (D7 lost this redirect to another rule).
 */
function vaxia_storyhosts_scene_form_submit(array $form, FormStateInterface $form_state) {
  $node = $form_state->getFormObject()->getEntity();
  \Drupal::messenger()->addStatus(t("Don't forget to set the scene to submitted so that the storyhost team can review it."));
  $form_state->setRedirectUrl(Url::fromUserInput('/node/' . $node->id() . '/workflow'));
}

/**
 * Implements hook_ENTITY_TYPE_insert() for workflow transitions: scene mails
 * (D7 vaxia_scene rules).
 */
function vaxia_storyhosts_scene_transition($transition): void {
  $node = $transition->getTargetEntity();
  if (!$node instanceof NodeInterface || $node->bundle() !== 'scene' || $transition->getFromSid() === $transition->getToSid()) {
    return;
  }
  $mailer = \Drupal::service('plugin.manager.mail');
  $footer = "\n\n~~~~~~\nThis email was automatically generated by [site:name].\nYou are receiving this email as part of your membership at [site:name].\nDo not reply this this email, it is not a monitored account and messages will not be received by a human.\n~~~~~~";
  $url = $node->toUrl('canonical', ['absolute' => TRUE])->toString();
  if ($transition->getToSid() === 'scene_evaluation_submitted') {
    $ids = \Drupal::entityQuery('user')->accessCheck(FALSE)->condition('status', 1)->condition('roles', 'administrator')->execute();
    $body = ' A scene is ready to be ruled.' . "\n\n"
      . 'You are receiving this email because you are one of the people available to rule scenes. Please take a moment to review this scene for any related XP / item / skill or other project as needed. When you rule this scene be sure to make note of it in the workflow comments so other Storyhosts know it has been taken care of and what the result was. Please do so before ' . date('m/d/Y', strtotime('+1 week')) . ".\n\n"
      . 'Scene: ' . $node->label() . "\nLink: " . $url . "\nThis scene is created by: " . $node->getOwner()->getDisplayName() . "\n\n"
      . 'If you are not comfortable with reviewing scenes at all, please speak to any of the current heads to update your access privileges.' . $footer;
    foreach (User::loadMultiple($ids) as $account) {
      $mailer->mail('vaxia_storyhosts', 'text', $account->getEmail(), $account->getPreferredLangcode(), ['subject' => ' [site:mail-prefix] A scene is ready to be ruled', 'body' => $body]);
    }
    \Drupal::messenger()->addStatus(t("Your scene has been submitted, and email sent to the Storyhost team to let them know to come take a look at it and get it ruled quickly. The SH staff sees the scenes in their notification windows and gets an email - so you don't need to post the scene on the Forums to get SH attention for it but if you haven't heard anything back within three days - please feel free to follow up on the forums!"));
  }
  if ($transition->getToSid() === 'scene_evaluation_ruled' && $node->getOwner()->isActive()) {
    $owner = $node->getOwner();
    $mailer->mail('vaxia_storyhosts', 'text', $owner->getEmail(), $owner->getPreferredLangcode(), [
      'subject' => '[site:mail-prefix] Your scene has been ruled',
      'body' => 'Your scene, ' . $node->label() . ", has been ruled!\n\nYour scene was ruled with the following notes: " . $transition->getComment() . "\n\nYou can find the scene at: " . $url . "\n\nIf you have additional questions, please contact: " . \Drupal::currentUser()->getDisplayName() . $footer,
    ]);
  }
}

/**
 * Implements hook_mail().
 */
function vaxia_storyhosts_mail($key, &$message, $params) {
  $message['subject'] = $params['subject'];
  $message['body'][] = $params['body'];
}
