<?php

namespace Drupal\vaxia_core\Form;

use Drupal\Core\Form\ConfigFormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\workflow\Entity\WorkflowState;

/**
 * Vaxia settings (D7 admin/vaxia/vaxia, _vaxia_admin_settings()). Form keys
 * keep the D7 variable names, which are also the config keys.
 */
class SettingsForm extends ConfigFormBase {

  public function getFormId() {
    return '_vaxia_admin_settings_form';
  }

  protected function getEditableConfigNames() {
    return ['vaxia_core.settings'];
  }

  /**
   * Plain settings: group => [key => [label, description]].
   */
  private const FIELDS = [
    'settings' => [
      'vaxia_starting_points' => ['Starting points', 'The starting points for a new character.'],
      'vaxia_starting_econ' => ['Starting Economy level', 'The starting Economy level for a new character.'],
      'vaxia_kxp_ratio' => ['KXP to XP ratio', 'The ratio of kxp to xp.'],
      'vaxia_max_kxp_post' => ['Max KXP per post', 'The maximum amount of kxp to be awarded per post.'],
      'vaxia_display_dice' => ['Dice rows to show', 'The number of dicerows to show in dice interface.'],
    ],
    'point_cost' => [
      'vaxia_attribute_cost' => ['Attribute cost multiplier', 'Attribute cost is calculated as (current/10) * multiplier.'],
      'vaxia_skill_cost' => ['Skill cost multiplier', 'Skill cost is calculated as (current/10) * multiplier.'],
      'vaxia_economy_cost' => ['Economy cost', 'Economy cost is calculated as (current/10) * multiplier.'],
      'vaxia_transfer_percentage' => ['Percent of XP', 'Percent of XP when transferred that goes to XP.'],
      'vaxia_transfer_cap' => ['Cap of XP', 'Amount of total XP that can be transferred, anything more is lost.'],
    ],
    'warn' => [
      'vaxia_pc_skill_warn' => ['Skill min warn', 'Warning on min skill value for a PC.'],
      'vaxia_pc_stat_spend_warn' => ['Stat spending warn', 'Warning spent on stats for a PC.'],
      'vaxia_pc_stat_high_warn' => ['Max caps warn', 'Warning on number of allowed max values for a PC.'],
      'vaxia_pc_stat_min_warn' => ['Max mins warn', 'Warning on number of allowed min values for a PC.'],
      'vaxia_pc_skill_ave_warn' => ['Skill average warn', 'Warning on average skill values for a PC.'],
    ],
    'block' => [
      'vaxia_pc_skill_min' => ['Skill min', 'Minimum allowed skill value for a PC.'],
      'vaxia_pc_skill_max' => ['Skill max', 'Maximum allowed skill value for a PC.'],
      'vaxia_pc_stat_min' => ['Stat min', 'Minimum allowed stat value for a PC.'],
      'vaxia_pc_stat_max' => ['Stat max', 'Maximum allowed stat value for a PC.'],
      'vaxia_pc_stat_spend_cap' => ['Stat spending cap', 'Maximum allowed to be spent on stats for a PC.'],
      'vaxia_pc_stat_high_cap' => ['Max caps', 'Maximum number of allowed max values for a PC.'],
      'vaxia_pc_stat_zero_cap' => ['Zero caps', 'Maximum number of zero skills for a PC.'],
      'vaxia_other_min_hxp' => ['Min HXP for others', 'Min HXP to be able to select Other and Conditions.'],
    ],
  ];

  public function buildForm(array $form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_core.settings');
    $groups = ['settings' => 'Settings', 'player_ui' => 'Player UI Settings', 'point_cost' => 'Point costs', 'workflows' => 'Workflow states', 'warn' => 'Warn creation', 'block' => 'Block creation'];
    foreach ($groups as $group => $title) {
      $form[$group] = ['#type' => 'details', '#title' => $title];
    }
    foreach (self::FIELDS as $group => $fields) {
      foreach ($fields as $key => [$label, $description]) {
        $form[$group][$key] = ['#type' => 'textfield', '#title' => $label, '#description' => $description, '#default_value' => $config->get($key)];
      }
    }
    $form['settings']['vaxia_xp_types'] = [
      '#type' => 'textarea',
      '#title' => 'XP types',
      '#description' => $this->t('Available XP sub-types. The following types used by the system and will be ignored if added here: @types', ['@types' => implode(', ', \Drupal\vaxia_core\Xp::MEANINGFUL)]),
      '#default_value' => $config->get('vaxia_xp_types'),
    ];
    $rooms = [];
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    foreach ($storage->loadMultiple($storage->getQuery()->accessCheck(FALSE)->condition('type', 'rpg_chatroom')->sort('title')->execute()) as $room) {
      $rooms[$room->id()] = $room->label();
    }
    $form['settings']['vaxia_limbo_room'] = [
      '#type' => 'select',
      '#title' => 'Limbo room',
      '#description' => $this->t('In this room, posts made do not gain XP.'),
      '#options' => $rooms,
      '#multiple' => TRUE,
      '#size' => 15,
      '#default_value' => array_values($config->get('vaxia_limbo_room') ?: []),
    ];
    $form['settings']['vaxia_mail_prefix'] = [
      '#type' => 'textfield',
      '#title' => 'Mail subject prefix',
      '#description' => $this->t('Site mails start their subject with this in brackets, e.g. "[My Site] Character approved". Empty: the site name.'),
      '#default_value' => $config->get('vaxia_mail_prefix'),
    ];
    $form['settings']['vaxia_dice_roller_stacking'] = [
      '#type' => 'checkbox',
      '#title' => 'Allow items to stack for dice rolling.',
      '#default_value' => $config->get('vaxia_dice_roller_stacking'),
    ];
    foreach (['vaxia_public_char_fields' => 'Publicly visible character fields.', 'vaxia_public_user_fields' => 'Publicly visible user fields.', 'vaxia_disabled_user_fields' => 'Disabled user fields.'] as $key => $label) {
      $form['player_ui'][$key] = ['#type' => 'textarea', '#title' => $label, '#default_value' => $config->get($key)];
    }
    $form['point_cost']['vaxia_transfer_cap_on'] = [
      '#type' => 'checkbox',
      '#title' => 'Cap XP to be transferred?',
      '#default_value' => $config->get('vaxia_transfer_cap_on'),
    ];
    $states = [];
    foreach (WorkflowState::loadMultiple() as $sid => $state) {
      if (!$state->isCreationState()) {
        $states[$sid] = $state->getWorkflow()->label() . ':' . $state->label();
      }
    }
    $form['workflows']['vaxia_transfer_workflow'] = [
      '#type' => 'select',
      '#title' => 'Transfer workflow',
      '#description' => 'Workflow state characters who are transferred from end up in.',
      '#options' => $states,
      '#empty_value' => '',
      '#default_value' => $config->get('vaxia_transfer_workflow'),
    ];
    $form['workflows']['vaxia_draft_workflow'] = [
      '#type' => 'select',
      '#title' => 'Draft workflow',
      '#description' => 'Workflow state for characters in creation.',
      '#options' => $states,
      '#empty_value' => '',
      '#default_value' => $config->get('vaxia_draft_workflow'),
    ];
    $form['workflows']['character_sheet_vaxia_playable'] = [
      '#type' => 'select',
      '#title' => 'Playable states',
      '#description' => 'States in which a character can be played (and is charged XP for changes).',
      '#options' => $states,
      '#multiple' => TRUE,
      '#default_value' => $config->get('character_sheet_vaxia_playable') ?: [],
    ];
    return parent::buildForm($form, $form_state);
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $config = $this->config('vaxia_core.settings');
    foreach (self::FIELDS as $fields) {
      foreach (array_keys($fields) as $key) {
        $config->set($key, (float) $form_state->getValue($key) == (int) $form_state->getValue($key) ? (int) $form_state->getValue($key) : (float) $form_state->getValue($key));
      }
    }
    foreach (['vaxia_xp_types', 'vaxia_public_char_fields', 'vaxia_public_user_fields', 'vaxia_disabled_user_fields', 'vaxia_transfer_workflow', 'vaxia_draft_workflow'] as $key) {
      $config->set($key, (string) $form_state->getValue($key));
    }
    $config->set('vaxia_mail_prefix', trim((string) $form_state->getValue('vaxia_mail_prefix')));
    $config->set('vaxia_dice_roller_stacking', (bool) $form_state->getValue('vaxia_dice_roller_stacking'));
    $config->set('vaxia_transfer_cap_on', (bool) $form_state->getValue('vaxia_transfer_cap_on'));
    $limbo = array_map('intval', array_values(array_filter((array) $form_state->getValue('vaxia_limbo_room'))));
    $config->set('vaxia_limbo_room', array_combine($limbo, $limbo) ?: []);
    $config->set('character_sheet_vaxia_playable', array_values(array_filter((array) $form_state->getValue('character_sheet_vaxia_playable'))));
    $config->save();
    parent::submitForm($form, $form_state);
  }

}
