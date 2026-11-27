<?php

/**
 * @file
 * The dice ruler (D7 dice_ruler.module): ruling forms for two-trait, combat
 * and magic actions, the player multi-attack advisor, and reading posted dice.
 *
 * The ruling functions are D7's, unchanged apart from the Drupal API calls
 * ($form_state values, settings, tables). The forms are served by
 * Drupal\vaxia_dice\Form\DiceRulerForm and PlayerAssistForm, the blocks by
 * the plugins in src/Plugin/Block.
 */

/**
 * A dice ruler setting (D7 variable name, same default as D7).
 */
function vaxia_dice_variable(string $name, $default = NULL) {
  $value = \Drupal::config('vaxia_dice.settings')->get($name);
  return $value ?? $default;
}

/**
 * Renders a table (D7 theme('table', ...)).
 */
function vaxia_dice_table(array $variables): string {
  // Cells hold markup (D7 printed it as is): keep it, minus scripts.
  $cell = function ($value) {
    if (is_array($value)) {
      if (isset($value['data']) && is_string($value['data'])) {
        $value['data'] = ['#markup' => $value['data']];
      }
      return $value;
    }
    return ['data' => ['#markup' => (string) $value]];
  };
  $rows = [];
  foreach ($variables['rows'] ?? [] as $key => $row) {
    $cells = isset($row['data']) && is_array($row['data']) ? $row['data'] : $row;
    $cells = array_map($cell, $cells);
    $rows[$key] = isset($row['data']) && is_array($row['data']) ? ['data' => $cells] + $row : $cells;
  }
  $table = ['#type' => 'table', '#header' => $variables['header'] ?? [], '#rows' => $rows];
  if (!empty($variables['attributes'])) {
    $table['#attributes'] = $variables['attributes'];
  }
  if (!empty($variables['caption'])) {
    $table['#caption'] = $variables['caption'];
  }
  if (isset($variables['empty'])) {
    $table['#empty'] = $variables['empty'];
  }
  return (string) \Drupal::service('renderer')->renderInIsolation($table);
}

/**
 * Weapon classes (weapon_category terms), in term order: name without its
 * "(…)" part, percentage and multi-action modifier.
 */
function _dice_ruler_weapon_class_info() {
  $storage = \Drupal::entityTypeManager()->getStorage('taxonomy_term');
  $classes = [];
  foreach ($storage->loadByProperties(['vid' => 'weapon_category']) as $tid => $term) {
    $weight = $term->getWeight() ?: $tid;
    while (isset($classes[$weight])) {
      $weight++;
    }
    $classes[$weight] = [
      'tid' => $tid,
      'name' => trim(explode('(', $term->label())[0]),
      'mods' => $term->hasField('field_multi_action_mod') ? ($term->get('field_multi_action_mod')->value ?: 0) : 0,
      'perc' => $term->hasField('field_percentage') ? ($term->get('field_percentage')->value ?: 0) : 0,
    ];
  }
  ksort($classes);
  return $classes;
}

function _dice_ruler_weapon_classes() {
  $classes = array();
  foreach (_dice_ruler_weapon_class_info() as $tid => $weapon_class) {
    $classes[$weapon_class['tid']] = $weapon_class['name'];
  }
  return $classes;
}

function _dice_ruler_weapon_percentages() {
  $classes = array();
  foreach (_dice_ruler_weapon_class_info() as $tid => $weapon_class) {
    $classes[$weapon_class['tid']] = $weapon_class['perc'];
  }
  return $classes;
}

function _dice_ruler_weapon_modifiers() {
  $classes = array();
  foreach (_dice_ruler_weapon_class_info() as $tid => $weapon_class) {
    $classes[$weapon_class['tid']] = $weapon_class['mods'];
  }
  return $classes;
}

function _dice_ruler_player_assist_form($form, &$form_state) {
  // D7 used an undefined $first here (a PHP notice); the first class is meant.
  $first = array_key_first(_dice_ruler_weapon_classes());
  // D7's collapsible, collapsed fieldset: D11's closed details element (D11
  // has no collapse.js for fieldsets, so it showed open).
  $form['multi_attack'] = array(
    '#title' => t('Should I try a multi-attack?'),
    '#type' => 'details',
    '#open' => FALSE,
    '#size' => 5,
  );
  $form['multi_attack']['multi_attack_str'] = array(
    '#title' => t('My strength is:'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 5,
  );
  $form['multi_attack']['multi_attack_dex'] = array(
    '#title' => t('My dexterity is:'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 5,
  );
  $form['multi_attack']['multi_attack_skill'] = array(
    '#title' => t('My skill is:'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#size' => 5,
  );
  $form['multi_attack']['multi_attack_type'] = array(
    '#title' => t('My weapon class is:'),
    '#type' => 'select',
    '#options' => _dice_ruler_weapon_classes(),
    '#required' => TRUE,
    '#default_value' => $first,
  );
  $form['multi_attack']['multi_attack_perc'] = array(
    '#title' => t('My added weapon percentage is:'),
    '#type' => 'textfield',
    '#default_value' => 0,
    '#size' => 5,
  );
  $form['multi_attack']['multi_attack_target_might'] = array(
    '#title' => t('My target\'s might is around:'),
    '#type' => 'textfield',
    '#required' => TRUE,
    '#default_value' => 50,
    '#size' => 5,
  );
  $form['multi_attack']['multi_attack_rule'] = array(
    '#type' => 'button',
    '#value' => t('Should I?'),
    '#ajax' => array(
      'callback' => '_dice_ruler_player_assist_multi_attack_form_ajax',
      'wrapper' => 'dice_ruler_dice_assist_multi_attack_results',
      'method' => 'replaceWith',
      'effect' => 'fade',
    ),
  );
  $form['multi_attack']['multi_attack_results'] = array(
    '#type' => 'item',
    '#markup' => _dice_ruler_dice_assist_multi_attack_results($form, $form_state),
    '#prefix' => '<div id="dice_ruler_dice_assist_multi_attack_results" style="padding-top:0.5em;">',
    '#suffix' => '</div>',
  );
  return $form;
}

function _dice_ruler_dice_assist_multi_attack_results($form, $form_state) {
  $values = $form_state->getValues();
  $advice = '';
  if (!empty($values['multi_attack_str'])) {
    // Ok, lets figure this out.
    $multi_attack_str = trim($values['multi_attack_str']);
    $multi_attack_dex = trim($values['multi_attack_dex']);
    $multi_attack_skill = trim($values['multi_attack_skill']);
    $multi_attack_perc = !empty($values['multi_attack_perc']) ? trim($values['multi_attack_perc']) : 0;
    $penalty_base = vaxia_dice_variable('dice_ruler_extra_action_penalty', 20);
    $tid = trim($values['multi_attack_type']);
    $percs = _dice_ruler_weapon_percentages();
    $multi_attack_perc = $multi_attack_perc + $percs[$tid];
    $multi_attack_perc = $multi_attack_perc/100;
    $multi_attack_map_mod = 0;
    $mods = _dice_ruler_weapon_modifiers();
    $multi_attack_map_mod = $mods[$tid];
    $multi_attack_target_might = trim($values['multi_attack_target_might']);
    $number_attacks = 0;
    $this_damage = 0;
    // D7 left this unset when the first attack already missed (a PHP notice).
    $last_damage = 0;
    $success = TRUE;
    while ($success) {
      $success = FALSE; // Assume no success.
      $number_attacks++; // Set number of attacks;
      $this_adjust = 0;
      if ($number_attacks > 1) {
        $this_adjust = ($penalty_base + $multi_attack_map_mod) * ($number_attacks - 1);// Adjust might.
      }
      // Now figure out if we can hit this guy more than once based on rolls of 50.
      $this_result = _dice_ruler_make_two_trait_ruling(
        $multi_attack_dex + $multi_attack_skill - $this_adjust,
        $multi_attack_target_might, // Defender.
        0,
        50,
        $multi_attack_str + $multi_attack_skill - $this_adjust,
        $multi_attack_target_might, // Defender.
        0,
        50
      );
      // Check if success.
      if ($this_result['success'] == 'yes') {
        $success = TRUE;
        $last_damage = $this_damage;
        $this_damage = $this_result['pp'] + ($this_result['pp'] * $multi_attack_perc);
      }
    }
    // Number of successful attacks.
    $number_attacks--;
    $average_might = $multi_attack_dex + $multi_attack_skill + $multi_attack_str + $multi_attack_skill;
    $average_might = $average_might / 2;
    $advice = t('With your average might of @average_might, and an opponent of might @multi_attack_target_might you can probably ' .
      'hit him @number_attacks time(s) for a total of @total_damage.',
      array('@average_might' => number_format($average_might), '@multi_attack_target_might' => $multi_attack_target_might,
      '@number_attacks' => $number_attacks, '@total_damage' => $this_damage * $number_attacks));
    $less_attacks = $number_attacks - 1;
    if (($last_damage * $less_attacks) > ($this_damage * $number_attacks)) {
      $advice .= ' ' . t('But you would do more damage if you attacked @number_attacks time(s) for @total_damage.',
        array('@number_attacks' => $less_attacks, '@total_damage' => $last_damage * $less_attacks));
    }
    $advice .= ' ' . t('This calculation assumes everyone rolls 50s. This is an estimate only: your individual results may vary, ' .
      'and A/SHs may apply circumstance modifiers.');
  }
  return $advice;
}

function _dice_ruler_player_assist_multi_attack_form_ajax($form, $form_state) {
  return $form['multi_attack']['multi_attack_results'];
}

function _dice_ruler_form_two_trait($i) {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Two trait'),
    '#states' => array(
      'visible' => array(
        ':input[name="roll_type_' . $i .'"]' => array('value' => 'two_trait'),
      ),
    ),
    '#attributes' => array(
      'class' => array('two-trait-form'),
    ),
  );
  $form['a'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trait A'),
  );
  $form['a']['two_trait_might_a_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['a']['two_trait_diff_a_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Diff.'),
    '#size' => 5,
    '#default_value' => vaxia_dice_variable('dice_ruler_default_diff', 25),
  );
  $form['a']['two_trait_rolled_a_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['b'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trait B'),
  );
  $form['b']['two_trait_might_b_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['b']['two_trait_diff_b_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Diff.'),
    '#size' => 5,
    '#default_value' => vaxia_dice_variable('dice_ruler_default_diff', 25),
  );
  $form['b']['two_trait_rolled_b_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  return $form;
}

function _dice_ruler_form_two_trait_vs($i) {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Two trait vs trait'),
    '#states' => array(
      'visible' => array(
        ':input[name="roll_type_' . $i .'"]' => array('value' => 'two_trait_vs'),
      ),
    ),
    '#attributes' => array(
      'class' => array('two-trait-vs-form'),
    ),
  );
  $form['a'] = array(
    '#type' => 'fieldset',
    '#title' => t('Character A'),
  );
  $form['a']['a'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trait A'),
  );
  $form['a']['a']['two_trait_vs_might_aa_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['a']['a']['two_trait_vs_rolled_aa_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['a']['b'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trait B'),
  );
  $form['a']['b']['two_trait_vs_might_ab_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['a']['b']['two_trait_vs_rolled_ab_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['b'] = array(
    '#type' => 'fieldset',
    '#title' => t('Character B'),
  );
  $form['b']['a'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trait A'),
  );
  $form['b']['a']['two_trait_vs_might_ba_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['b']['a']['two_trait_vs_rolled_ba_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['b']['b'] = array(
    '#type' => 'fieldset',
    '#title' => t('Trait B'),
  );
  $form['b']['b']['two_trait_vs_might_bb_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['b']['b']['two_trait_vs_rolled_bb_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  return $form;
}

function _dice_ruler_form_combat($i) {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Combat'),
    '#states' => array(
      'visible' => array(
        ':input[name="roll_type_' . $i .'"]' => array('value' => 'combat'),
      ),
    ),
    '#attributes' => array(
      'class' => array('combat-form'),
    ),
  );

  // Combatant A's attack.
  $form['a'] = array(
    '#type' => 'fieldset',
    '#title' => t('Character A'),
  );
  $form['a']['combat_weapon_a_' . $i] = array(
    '#type' => 'radios',
    '#title' => t('Weapon Class'),
    '#options' => _dice_ruler_weapon_classes(),
  );
  $form['a']['attack_a_'.$i]['dex'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dexterity'),
  );
  $form['a']['attack_a_'.$i]['dex']['combat_might_a_dex_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['a']['attack_a_'.$i]['dex']['combat_rolled_a_dex_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['a']['attack_a_'.$i]['str'] = array(
    '#type' => 'fieldset',
    '#title' => t('Strength'),
  );
  $form['a']['attack_a_'.$i]['str']['combat_might_a_str_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['a']['attack_a_'.$i]['str']['combat_rolled_a_str_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['a']['attack_a_'.$i]['end'] = array(
    '#type' => 'fieldset',
    '#title' => t('Endurance'),
  );
  $form['a']['attack_a_'.$i]['end']['combat_might_a_end_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['a']['attack_a_'.$i]['end']['combat_rolled_a_end_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );

  // Combatant B's attack.
  $form['b'] = array(
    '#type' => 'fieldset',
    '#title' => t('Character B'),
  );
  // Toggle between weapon percents and weapon categories.
  $number_actions_b = array(
    '1' => t('One action'),
    '2' => t('Two actions'),
    '3' => t('Three actions'),
    '4' => t('Four actions'),
  );
  $form['b']['combat_weapon_b_' . $i] = array(
    '#type' => 'radios',
    '#title' => t('Weapon Class'),
    '#options' => _dice_ruler_weapon_classes(),
  );
  $form['b']['number_actions_b_' . $i] = array(
    '#type' => 'select',
    '#title' => t('# Actions'),
    '#options' => $number_actions_b,
    '#default_value' => 1,
    '#description' => t('Including defensive and non-combat actions.'),
  );
  $form['b']['attack_'.$i]['dex'] = array(
    '#type' => 'fieldset',
    '#title' => t('Dexterity'),
  );
  $form['b']['attack_'.$i]['dex']['combat_might_b_dex_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['b']['attack_'.$i]['dex']['combat_rolled_b_dex_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['b']['attack_'.$i]['str'] = array(
    '#type' => 'fieldset',
    '#title' => t('Strength'),
  );
  $form['b']['attack_'.$i]['str']['combat_might_b_str_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['b']['attack_'.$i]['str']['combat_rolled_b_str_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['b']['attack_'.$i]['end'] = array(
    '#type' => 'fieldset',
    '#title' => t('Endurance'),
  );
  $form['b']['attack_'.$i]['end']['combat_might_b_end_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['b']['attack_'.$i]['end']['combat_rolled_b_end_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  return $form;
}

function _dice_ruler_form_magic($i) {
  $form = array(
    '#type' => 'fieldset',
    '#title' => t('Magic'),
    '#states' => array(
      'visible' => array(
        ':input[name="roll_type_' . $i .'"]' => array('value' => 'magic'),
      ),
    ),
    '#attributes' => array(
      'class' => array('magic-form'),
    ),
  );
  $form['a'] = array(
    '#type' => 'fieldset',
    '#title' => t('Intelligence'),
  );
  $form['a']['magic_might_a_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['a']['magic_diff_a_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Diff.'),
    '#size' => 5,
    '#default_value' => vaxia_dice_variable('dice_ruler_default_diff', 25),
  );
  $form['a']['magic_rolled_a_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['b'] = array(
    '#type' => 'fieldset',
    '#title' => t('Spirituality'),
  );
  $form['b']['magic_might_b_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Might'),
    '#size' => 5,
  );
  $form['b']['magic_diff_b_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Diff.'),
    '#size' => 5,
    '#default_value' => vaxia_dice_variable('dice_ruler_default_diff', 25),
  );
  $form['b']['magic_rolled_b_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Rolled'),
    '#size' => 5,
  );
  $form['target'] = array(
    '#type' => 'fieldset',
    '#title' => t('Target Resistance'),

  );
  $form['target']['magic_target_stat_int_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Int -OR- Dex'),
    '#size' => 5,
  );
  $form['target']['magic_target_stat_spi_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Spi -OR- End'),
    '#size' => 5,
  );
  $form['target']['magic_target_skill_' . $i] = array(
    '#type' => 'textfield',
    '#title' => t('Skill*'),
    '#size' => 5,
  );
  return $form;
}

function dice_ruler_form($form, &$form_state) {
  $values = $form_state->getValues();
  $form = array();
  // Show help if we have it.
  $general_help = vaxia_dice_variable('dice_ruler_help_general', '');
  if (!empty($general_help)) {
    $form['dice_ruler_dice_help'] = array(
    '#title' => t('General Help'),
    '#type' => 'fieldset',
    '#prefix' => '<div id="dice_ruler_dice_help" class="dice-ruler-help dice_ruler_dice_help">',
    '#suffix' => '</div>',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#states' => array(
      'invisible' => array(
        ':input[name="actions"]' => array('value' => 'hidden'),
        ),
      ),
    'help' => array(
      '#type' => 'item',
      '#markup' => $general_help,
     ),
    );
  }
  $types = array('two_trait', 'two_trait_vs', 'combat', 'magic');
  foreach ($types as $type) {
    $help = vaxia_dice_variable('dice_ruler_help_' . $type, '');
    if (!empty($help)) {
      $form['dice_ruler_dice_help_' . $type] = array(
        '#title' => ucwords(str_replace('_', ' ', $type)) . ' ' . t('Help'),
        '#type' => 'fieldset',
        '#prefix' => '<div id="dice_ruler_dice_help_' . $type . '" class="dice-ruler-help secondary-help">',
        '#suffix' => '</div>',
        '#collapsible' => TRUE,
        '#collapsed' => TRUE,
        '#states' => array(
          'invisible' => array(
            ':input[name="actions"]' => array('value' => 'hidden'),
            ),
          ),
        );
      $form['dice_ruler_dice_help_' . $type]['help'] = array(
        '#type' => 'item',
        '#markup' => $help,
        );
    }
  }
  // Find out how many actions we may have selected.
  $number_actions = !empty($values['actions']) ? $values['actions'] : 0;
  // Display request for # of actions.
  $penalty_base = vaxia_dice_variable('dice_ruler_extra_action_penalty', 20);
  $form['actions'] = array(
    '#type' => 'select',
    '#options' => array(
      'hidden' => t('-- Select number of actions to rule --'),
      '1' => '1 action - no multi action penalty',
      '2' => '2 action - ' . ($penalty_base * -1) . ' multi action penalty',
      '3' => '3 action - ' . ($penalty_base * -2) . ' multi action penalty',
      '4' => '4 action - ' . ($penalty_base * -3) . ' multi action penalty',
    ),
    '#default_value' => 'hidden',
  );
  $values = array();
  for ($i = 1; $i < 5; $i++) {
    $values[] = array('value' => $i);
  }
  // Display a form for each action.
  for ($i = 1; $i < 5; $i++) {
    $form['action_' . $i] = array(
      '#type' => 'fieldset',
      '#title' => t('Action #' . $i),
      '#states' => array(
        'visible' => array(
          ':input[name="actions"]' => $values,
        ),
      ),
    );
    array_shift($values); // Clip an action off the array of visible.
    // Select a type for this action.
    $form['action_' . $i]['roll_type_' . $i] = array(
      '#type' => 'select',
      '#options' => array(
        'two_trait' => t('Two trait'),
        'two_trait_vs' => t('Two trait vs. two trait'),
        'combat' => t('Combat'),
        'magic' => t('Magic'),
      ),
    );
    // Subforms for each type of roll
    $form['action_' . $i]['two_trait_' . $i] = _dice_ruler_form_two_trait($i);
    $form['action_' . $i]['two_trait_vs_' . $i] = _dice_ruler_form_two_trait_vs($i);
    $form['action_' . $i]['combat_' . $i] = _dice_ruler_form_combat($i);
    $form['action_' . $i]['magic_' . $i] = _dice_ruler_form_magic($i);
  }
  // Provide button.
  $form['rule_dice'] = array(
    '#type' => 'button',
    '#value' => t('Rule roll'),
    '#ajax' => array(
      'callback' => 'dice_ruler_form_ajax',
      'wrapper' => 'dice_ruler_dice_results',
      'method' => 'replaceWith',
      'effect' => 'fade',
    ),
    '#states' => array(
      'invisible' => array(
        ':input[name="actions"]' => array('value' => 'hidden'),
      ),
    ),
  );
  // Show results!
  $results = array();
  for ($i=1; $i < $number_actions + 1; $i++) {
    $results[] = '<label for="dice-ruler-dice-result">' . t('Action #') . $i . '</label>' .
      '<div class="dice-ruler-dice-result">' .
      _dice_ruler_form_get_results_for_action($form, $form_state, $i, $number_actions) .
      '</div>';
  }
  $form['dice_ruler_dice_results'] = array(
    '#type' => 'item',
    '#markup' => implode('</br>', $results),
    '#prefix' => '<div id="dice_ruler_dice_results">',
    '#suffix' => '</div>',
    '#states' => array(
      'invisible' => array(
        ':input[name="actions"]' => array('value' => 'hidden'),
      ),
    ),
  );
  $form['#attached']['library'][] = 'vaxia_dice/dice_ruler';
  return $form;
}

function _dice_ruler_form_get_results_for_action($form, $form_state, $i, $number_actions) {
  $values = $form_state->getValues();
  $penalty_base = vaxia_dice_variable('dice_ruler_extra_action_penalty', 20);
  $action_penalty = $penalty_base * ($number_actions - 1);

  // Gather the results by action number.
  $results = t('No roll performed.');
  if (!empty($values['roll_type_' . $i])) {
    $results = '';
    switch ($values['roll_type_' . $i]) {
      case 'two_trait':
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_a = is_numeric(trim((string) $values['two_trait_might_a_' . $i])) ? max(1, $values['two_trait_might_a_' . $i] - $action_penalty) : $values['two_trait_might_a_' . $i];
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_b = is_numeric(trim((string) $values['two_trait_might_b_' . $i])) ? max(1, $values['two_trait_might_b_' . $i] - $action_penalty) : $values['two_trait_might_b_' . $i];
        $results .= _dice_ruler_two_trait(
          trim($new_might_a),
          trim($values['two_trait_diff_a_' . $i]),
          trim($values['two_trait_add_diff_a_' . $i]),
          trim($values['two_trait_rolled_a_' . $i]),
          trim($new_might_b),
          trim($values['two_trait_diff_b_' . $i]),
          trim($values['two_trait_add_diff_b_' . $i]),
          trim($values['two_trait_rolled_b_' . $i])
        );
      break;
      case 'two_trait_vs':
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_aa = is_numeric(trim((string) $values['two_trait_vs_might_aa_' . $i])) ? max(1, $values['two_trait_vs_might_aa_' . $i] - $action_penalty) : $values['two_trait_vs_might_aa_' . $i];
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_ab = is_numeric(trim((string) $values['two_trait_vs_might_ab_' . $i])) ? max(1, $values['two_trait_vs_might_ab_' . $i] - $action_penalty) : $values['two_trait_vs_might_ab_' . $i];
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_ba = is_numeric(trim((string) $values['two_trait_vs_might_ba_' . $i])) ? max(1, $values['two_trait_vs_might_ba_' . $i] - $action_penalty) : $values['two_trait_vs_might_ba_' . $i];
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_bb = is_numeric(trim((string) $values['two_trait_vs_might_bb_' . $i])) ? max(1, $values['two_trait_vs_might_bb_' . $i] - $action_penalty) : $values['two_trait_vs_might_bb_' . $i];
        $results .= _dice_ruler_two_trait_vs(
          trim($new_might_aa),
          trim($values['two_trait_vs_add_diff_aa_' . $i]),
          trim($values['two_trait_vs_rolled_aa_' . $i]),
          trim($new_might_ab),
          trim($values['two_trait_vs_add_diff_ab_' . $i]),
          trim($values['two_trait_vs_rolled_ab_' . $i]),
          trim($new_might_ba),
          trim($values['two_trait_vs_add_diff_ba_' . $i]),
          trim($values['two_trait_vs_rolled_ba_' . $i]),
          trim($new_might_bb),
          trim($values['two_trait_vs_add_diff_bb_' . $i]),
          trim($values['two_trait_vs_rolled_bb_' . $i])
        );
      break;
      case 'combat':
        // If we use classes, look up and set the correct class based weapon value.
        $a_type = $values['combat_weapon_a_' . $i];
        $b_type = $values['combat_weapon_b_' . $i];
        $add_a = 0;
        $add_b = 0;
        $percs = _dice_ruler_weapon_percentages();
        $values['combat_weapon_a_' . $i] = $add_a + $percs[$a_type];
        $values['combat_weapon_b_' . $i] = $add_b + $percs[$b_type];
        if ($values['combat_weapon_a_' . $i] != 0) {
          $results .= '<div class="dice-ruler-dice-multi-action-alert">' . t('Character A, additional damage percent: %pen%',
              array('%pen' => $values['combat_weapon_a_' . $i])) . '</div>';
        }
        if ($values['combat_weapon_b_' . $i] != 0) {
          $results .= '<div class="dice-ruler-dice-multi-action-alert">' . t('Character B, additional damage percent: %pen%',
              array('%pen' => $values['combat_weapon_b_' . $i])) . '</div>';
        }
        // Figure out the penalties for character b.
        $number_actions_b = !empty($values['number_actions_b_' . $i]) ? trim($values['number_actions_b_' . $i]) : 1;
        $action_penalty_b = $penalty_base * ($number_actions_b - 1);
        // Default mod is zero.
        $mod_base_a = $mod_base_b = 0;

        // Based on the weapon, figure out the modifier for a and b.
        $mods = _dice_ruler_weapon_modifiers();
        $mod_base_a = $mods[$a_type];
        // Display mod.
        $penalty_base_str = array();
        for ($this_attack = 1; $this_attack < $number_actions; $this_attack++) {
          $penalty_base_str[] = ($penalty_base + $mod_base_a) * ($this_attack);
        }
        $penalty_base_str = implode(' / ', $penalty_base_str);
        if ($number_actions > 1) {
          $results .= '<div class="dice-ruler-dice-multi-action-alert">' .
            t('Character A taking %num_actions actions is at multi action penalties for all actions: %penalty_base_str',
              array('%num_actions' => $number_actions, '%penalty_base_str' => $penalty_base_str)) . '</div>';
        }
        else {
          $results .= '<div class="dice-ruler-dice-multi-action-alert">' .
            t('Character A taking %num_actions actions and is at no penalty.',
              array('%num_actions' => $number_actions)) . '</div>';
        }
        // And recalculate the correct modifier to apply.
        $action_penalty = ($penalty_base + $mod_base_a) * ($number_actions - 1);

        // Based on the weapon, figure out the modifier for a and b.
        $mod_base_b = $mods[$b_type];
        // Display mod.
        $penalty_base_str = array();
        for ($this_attack = 1; $this_attack < $number_actions_b; $this_attack++) {
          $penalty_base_str[] = ($penalty_base + $mod_base_b) * ($this_attack);
        }
        $penalty_base_str = implode(' / ', $penalty_base_str);
        if ($number_actions_b > 1) {
          $results .= '<div class="dice-ruler-dice-multi-action-alert">' .
            t('Character B taking %num_actions actions is at multi action penalties for all actions: %penalty_base_str',
              array('%num_actions' => $number_actions_b, '%penalty_base_str' => $penalty_base_str)) . '</div>';
        }
        else {
          $results .= '<div class="dice-ruler-dice-multi-action-alert">' .
            t('Character B taking %num_actions actions and is at no penalty.',
              array('%num_actions' => $number_actions_b)) . '</div>';
        }
        // And recalculate the correct modifier to apply.
        $action_penalty_b = ($penalty_base + $mod_base_b) * ($number_actions_b - 1);

        // Character A.
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_dex = is_numeric(trim((string) $values['combat_might_a_dex_' . $i])) ? max(1, $values['combat_might_a_dex_' . $i] - $action_penalty) : $values['combat_might_a_dex_' . $i];
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_str = is_numeric(trim((string) $values['combat_might_a_str_' . $i])) ? max(1, $values['combat_might_a_str_' . $i] - $action_penalty) : $values['combat_might_a_str_' . $i];
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_end = is_numeric(trim((string) $values['combat_might_a_end_' . $i])) ? max(1, $values['combat_might_a_end_' . $i] - $action_penalty) : $values['combat_might_a_end_' . $i];
        // Character B.
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_dex_b = is_numeric(trim((string) $values['combat_might_b_dex_' . $i])) ? max(1, $values['combat_might_b_dex_' . $i] - $action_penalty_b) : $values['combat_might_b_dex_' . $i];
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_str_b = is_numeric(trim((string) $values['combat_might_b_str_' . $i])) ? max(1, $values['combat_might_b_str_' . $i] - $action_penalty_b) : $values['combat_might_b_str_' . $i];
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_end_b = is_numeric(trim((string) $values['combat_might_b_end_' . $i])) ? max(1, $values['combat_might_b_end_' . $i] - $action_penalty_b) : $values['combat_might_b_end_' . $i];
        // Get results.
        $results .= _dice_ruler_combat(
          // Character A.
          trim($new_might_dex),
          trim($values['combat_add_diff_a_dex_' . $i]),
          trim($values['combat_rolled_a_dex_' . $i]),
          trim($new_might_str),
          trim($values['combat_add_diff_a_str_' . $i]),
          trim($values['combat_rolled_a_str_' . $i]),
          trim($new_might_end),
          trim($values['combat_add_diff_a_end_' . $i]),
          trim($values['combat_rolled_a_end_' . $i]),
          trim($values['combat_weapon_a_' . $i]),
          // And the opposition.
          trim($new_might_dex_b),
          trim($values['combat_add_diff_b_dex_' . $i]),
          trim($values['combat_rolled_b_dex_' . $i]),
          trim($new_might_str_b),
          trim($values['combat_add_diff_b_str_' . $i]),
          trim($values['combat_rolled_b_str_' . $i]),
          trim($new_might_end_b),
          trim($values['combat_add_diff_b_end_' . $i]),
          trim($values['combat_rolled_b_end_' . $i]),
          trim($values['combat_weapon_b_' . $i])
        );
      break;
      case 'magic':
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_a = is_numeric(trim((string) $values['magic_might_a_' . $i])) ? max(1, $values['magic_might_a_' . $i] - $action_penalty) : $values['magic_might_a_' . $i];
        // Only numbers take the multi-action penalty: text and blanks go on to
        // the ruling's own "not numeric" / "missing" checks (D7 turned them into 1).
        $new_might_b = is_numeric(trim((string) $values['magic_might_b_' . $i])) ? max(1, $values['magic_might_b_' . $i] - $action_penalty) : $values['magic_might_b_' . $i];
        $results .= _dice_ruler_magic(
          trim($new_might_a),
          trim($values['magic_diff_a_' . $i]),
          trim($values['magic_add_diff_a_' . $i]),
          trim($values['magic_rolled_a_' . $i]),
          trim($new_might_b),
          trim($values['magic_diff_b_' . $i]),
          trim($values['magic_add_diff_b_' . $i]),
          trim($values['magic_rolled_b_' . $i]),
          trim($values['magic_target_stat_int_' . $i]),
          trim($values['magic_target_stat_spi_' . $i]),
          trim($values['magic_target_skill_' . $i])
        );
      break;
    }
  }
  return $results;
}

function dice_ruler_form_ajax($form, $form_state) {
  return $form['dice_ruler_dice_results'];
}

function _dice_ruler_two_trait(
  $two_trait_might_a,
  $two_trait_diff_a,
  $two_trait_add_diff_a,
  $two_trait_rolled_a,
  $two_trait_might_b,
  $two_trait_diff_b,
  $two_trait_add_diff_b,
  $two_trait_rolled_b
) {
  // Check that all are numeric.
  if (!empty($two_trait_might_a) && !is_numeric($two_trait_might_a)
    || !empty($two_trait_diff_a) && !is_numeric($two_trait_diff_a)
    || !empty($two_trait_add_diff_a) && !is_numeric($two_trait_add_diff_a)
    || !empty($two_trait_rolled_a) && !is_numeric($two_trait_rolled_a)) {
    return t('Not all values given were numeric. We need numbers for math.');
  }
  // Check our required values are set.
  if (empty($two_trait_might_a)
    || empty($two_trait_diff_a)
    || empty($two_trait_rolled_a)) {
    return t('Not all required values given, or values are zero.');
  }
  // Set default if empty.
  if (empty($two_trait_add_diff_a)) {
    $two_trait_add_diff_a = 0;
  }
  // Check that all are numeric.
  if (!empty($two_trait_might_b) && !is_numeric($two_trait_might_b)
    || !empty($two_trait_diff_b) && !is_numeric($two_trait_diff_b)
    || !empty($two_trait_add_diff_b) && !is_numeric($two_trait_add_diff_b)
    || !empty($two_trait_rolled_b) && !is_numeric($two_trait_rolled_b)) {
    return t('Not all values given were numeric. We need numbers for math.');
  }
  // Check our required values are set.
  if (empty($two_trait_might_b)
    || empty($two_trait_diff_b)
    || empty($two_trait_rolled_b)) {
    return t('Not all required values given, or values are zero.');
  }
  // Set default if empty.
  if (empty($two_trait_add_diff_b)) {
    $two_trait_add_diff_b = 0;
  }
  $results = _dice_ruler_make_two_trait_ruling(
    $two_trait_might_a,
    $two_trait_diff_a, // Difficulty.
    $two_trait_add_diff_a,
    $two_trait_rolled_a,
    $two_trait_might_b,
    $two_trait_diff_b, // Difficulty.
    $two_trait_add_diff_b,
    $two_trait_rolled_b
  );
  // Now put all these into a table to display.
  $header = array(
    t('Might'),
    t('Diff'),
    t('Rolled'),
    t('Roll needed'),
    t('Success'),
    t('Winsby'),
    t('Points'),
    t('1/7th PP'),
    t('% of diff'),
    t('Failed might'),
  );
  $rows[] = $results;
  $output = vaxia_dice_table(array(
    'header' => $header,
    'rows' => $rows,
  ));
  return $output;
}

function _dice_ruler_two_trait_vs(
  $two_trait_vs_might_a_a, // value_characterID_traitID
  $two_trait_vs_add_diff_a_a,
  $two_trait_vs_rolled_a_a,
  $two_trait_vs_might_a_b,
  $two_trait_vs_add_diff_a_b,
  $two_trait_vs_rolled_a_b,
  $two_trait_vs_might_b_a,
  $two_trait_vs_add_diff_b_a,
  $two_trait_vs_rolled_b_a,
  $two_trait_vs_might_b_b,
  $two_trait_vs_add_diff_b_b,
  $two_trait_vs_rolled_b_b
) {
  // Check that all are numeric.
  if (!empty($two_trait_vs_might_a_a) && !is_numeric($two_trait_vs_might_a_a)
    || !empty($two_trait_vs_add_diff_a_a) && !is_numeric($two_trait_vs_add_diff_a_a)
    || !empty($two_trait_vs_rolled_a_a) && !is_numeric($two_trait_vs_rolled_a_a)
  	|| !empty($two_trait_vs_might_a_b) && !is_numeric($two_trait_vs_might_a_b)
    || !empty($two_trait_vs_add_diff_a_b) && !is_numeric($two_trait_vs_add_diff_a_b)
    || !empty($two_trait_vs_rolled_a_b) && !is_numeric($two_trait_vs_rolled_a_b)
    ) {
    return t('Not all values given were numeric. We need numbers for math.');
  }
  // Check our required values are set.
  if (empty($two_trait_vs_might_a_a)
    || empty($two_trait_vs_rolled_a_a)
    || empty($two_trait_vs_might_a_b)
    || empty($two_trait_vs_rolled_a_b)) {
    return t('Not all required values given, or values are zero.');
  }
  // Set default if empty.
  if (empty($two_trait_vs_add_diff_a_a) || empty($two_trait_vs_add_diff_a_b)) {
    $two_trait_vs_add_diff_a_a = 0;
    $two_trait_vs_add_diff_a_b = 0;
  }
  // Check that all are numeric.
  if (!empty($two_trait_vs_might_b_a) && !is_numeric($two_trait_vs_might_b_a)
    || !empty($two_trait_vs_add_diff_b_a) && !is_numeric($two_trait_vs_add_diff_b_a)
    || !empty($two_trait_vs_rolled_b_a) && !is_numeric($two_trait_vs_rolled_b_a)
    || !empty($two_trait_vs_might_b_b) && !is_numeric($two_trait_vs_might_b_b)
    || !empty($two_trait_vs_add_diff_b_b) && !is_numeric($two_trait_vs_add_diff_b_b)
    || !empty($two_trait_vs_rolled_b_b) && !is_numeric($two_trait_vs_rolled_b_b)) {
    return t('Not all values given were numeric. We need numbers for math.');
  }
  // Check our required values are set.
  if (empty($two_trait_vs_might_b_a)
    || empty($two_trait_vs_rolled_b_a)
    || empty($two_trait_vs_might_b_b)
    || empty($two_trait_vs_rolled_b_b)) {
    return t('Not all required values given, or values are zero.');
  }
  // Set default if empty.
  if (empty($two_trait_vs_add_diff_b_a) || empty($two_trait_vs_add_diff_b_b)) {
    $two_trait_vs_add_diff_b_a = 0;
    $two_trait_vs_add_diff_b_b = 0;
  }
  // Setup for the tables.
  $header = array(
    t('Might'),
    t('Diff'),
    t('Rolled'),
    t('Roll needed'),
    t('Success'),
    t('Winsby'),
    t('Points'),
    t('1/7th PP'),
    t('% of diff'),
    t('Failed might'),
  );
  $output = '';
  $output .= '<div><b>' . t('First character\'s results.') . '</b></div>';
  // Get results from our master function.
  $results = _dice_ruler_make_two_trait_ruling(
  $two_trait_vs_might_a_a,
  $two_trait_vs_might_b_a, // DIFF
  $two_trait_vs_add_diff_a_a,
  $two_trait_vs_rolled_a_a,
  $two_trait_vs_might_a_b,
  $two_trait_vs_might_b_b, // DIFF
  $two_trait_vs_add_diff_a_b,
  $two_trait_vs_rolled_a_b
  );
  $rows[] = $results;
  // Now put all these into a table to display.
  $output .= vaxia_dice_table(array(
    'header' => $header,
    'rows' => $rows,
  ));
  $rows = array();
  // Second roll.
  $output .= '<div><b>' . t('Second character\'s results.') . '</b></div>';
  $results = _dice_ruler_make_two_trait_ruling(
  $two_trait_vs_might_b_a,
  $two_trait_vs_might_a_a, // DIFF
  $two_trait_vs_add_diff_b_a,
  $two_trait_vs_rolled_b_a,
  $two_trait_vs_might_b_b,
  $two_trait_vs_might_a_b, // DIFF
  $two_trait_vs_add_diff_b_b,
  $two_trait_vs_rolled_b_b
  );
  $rows[] = $results;
  // Now put all these into a table to display.
  $output .= vaxia_dice_table(array(
    'header' => $header,
    'rows' => $rows,
  ));
  return $output;
}

function _dice_ruler_combat(
  $combat_might_a_dex,
  $combat_add_diff_a_dex,
  $combat_rolled_a_dex,
  $combat_might_a_str,
  $combat_add_diff_a_str,
  $combat_rolled_a_str,
  $combat_might_a_end,
  $combat_add_diff_a_end,
  $combat_rolled_a_end,
  $combat_weapon_a,
  $combat_might_b_dex,
  $combat_add_diff_b_dex,
  $combat_rolled_b_dex,
  $combat_might_b_str,
  $combat_add_diff_b_str,
  $combat_rolled_b_str,
  $combat_might_b_end,
  $combat_add_diff_b_end,
  $combat_rolled_b_end,
  $combat_weapon_b
) {
  // Check that all are numeric.
  if (!empty($combat_might_a_dex) && !is_numeric($combat_might_a_dex)
    || !empty($combat_add_diff_a_dex) && !is_numeric($combat_add_diff_a_dex)
    || !empty($combat_rolled_a_dex) && !is_numeric($combat_rolled_a_dex)
    || !empty($combat_might_a_str) && !is_numeric($combat_might_a_str)
    || !empty($combat_add_diff_a_str) && !is_numeric($combat_add_diff_a_str)
    || !empty($combat_rolled_a_str) && !is_numeric($combat_rolled_a_str)
    || !empty($combat_might_a_end) && !is_numeric($combat_might_a_end)
    || !empty($combat_add_diff_a_end) && !is_numeric($combat_add_diff_a_end)
    || !empty($combat_rolled_a_end) && !is_numeric($combat_rolled_a_end)
    || !empty($combat_weapon_a) && !is_numeric($combat_weapon_a)
    ) {
    return t('Not all values given were numeric. We need numbers for math.');
  }
  // Check our required values are set.
  if (empty($combat_might_a_dex)
    || empty($combat_might_a_str)
    || empty($combat_might_a_end)) {
    return t('Not all required values given, or values are zero.');
  }
  // It is not possible in an un-even contest for the might to be zero. They will always have at least a might.
  // But they might not have a roll - if so, default to zero.
  if (empty($combat_rolled_a_dex)) {
    $combat_rolled_a_dex = 1;
  }
  if (empty($combat_rolled_a_str)) {
    $combat_rolled_a_str = 1;
  }
  if (empty($combat_rolled_a_end)) {
    $combat_rolled_a_end = 1;
  }
  // Set default if empty.
  if (empty($combat_add_diff_a_dex)) {
    $combat_add_diff_a_dex = 0;
  }
  if (empty($combat_add_diff_a_str)) {
    $combat_add_diff_a_str = 0;
  }
  if (empty($combat_add_diff_a_end)) {
    $combat_add_diff_a_end = 0;
  }
  // Check that all are numeric.
  if (!empty($combat_might_b_dex) && !is_numeric($combat_might_b_dex)
    || !empty($combat_add_diff_b_dex) && !is_numeric($combat_add_diff_b_dex)
    || !empty($combat_rolled_b_dex) && !is_numeric($combat_rolled_b_dex)
    || !empty($combat_might_b_str) && !is_numeric($combat_might_b_str)
    || !empty($combat_add_diff_b_str) && !is_numeric($combat_add_diff_b_str)
    || !empty($combat_rolled_b_str) && !is_numeric($combat_rolled_b_str)
    || !empty($combat_might_b_end) && !is_numeric($combat_might_b_end)
    || !empty($combat_add_diff_b_end) && !is_numeric($combat_add_diff_b_end)
    || !empty($combat_rolled_b_end) && !is_numeric($combat_rolled_b_end)
    || !empty($combat_weapon_b) && !is_numeric($combat_weapon_b)
    ) {
    return t('Not all values given were numeric. We need numbers for math.');
  }
  // Check our required values are set.
  if (!isset($combat_might_b_dex)
    || !isset($combat_might_b_str)
    || !isset($combat_might_b_end)) {
    return t('Not all required values given, or values are zero.');
  }
  // It is not possible in an un-even contest for the might to be zero. They will always have at least a might.
  // But they might not have a roll - if so, default to zero.
  if (empty($combat_rolled_b_dex)) {
    $combat_rolled_b_dex = 1;
  }
  if (empty($combat_rolled_b_str)) {
    $combat_rolled_b_str = 1;
  }
  if (empty($combat_rolled_b_end)) {
    $combat_rolled_b_end = 1;
  }
  // Set default if empty.
  if (empty($combat_add_diff_b_dex)) {
    $combat_add_diff_b_dex = 0;
  }
  if (empty($combat_add_diff_b_str)) {
    $combat_add_diff_b_str = 0;
  }
  if (empty($combat_add_diff_b_end)) {
    $combat_add_diff_b_end = 0;
  }
  // Prepare weapon combat damages
  if (empty($combat_weapon_a)) {
    $combat_weapon_a = 0;
  }
  elseif($combat_weapon_a > 1)  {
    $combat_weapon_a = $combat_weapon_a / 100;
  }
  if (empty($combat_weapon_b)) {
    $combat_weapon_b = 0;
  }
  elseif($combat_weapon_b > 1)  {
    $combat_weapon_b = $combat_weapon_b / 100;
  }

  // And make the rolls. The second value in a set (labeled defender) is always the difficulty.
  $attack_a_vs_b = _dice_ruler_make_two_trait_ruling(
    $combat_might_a_dex,
    $combat_might_b_dex, // Defender.
    $combat_add_diff_a_dex,
    $combat_rolled_a_dex,
    $combat_might_a_str,
    $combat_might_b_end, // Defender.
    $combat_add_diff_a_str,
    $combat_rolled_a_str
  );
  array_unshift($attack_a_vs_b, 'A Attacks B:');
  $remove_field = array_pop($attack_a_vs_b);
  $remove_field = array_pop($attack_a_vs_b);
  $attack_a_vs_b[] = ($attack_a_vs_b['pp'] + ($attack_a_vs_b['pp'] * $combat_weapon_a)) . t(' to B, w/ weapon');
  $rows[] = $attack_a_vs_b;
  $attack_b_vs_a = _dice_ruler_make_two_trait_ruling(
    $combat_might_b_dex,
    $combat_might_a_dex, // Defender.
    $combat_add_diff_b_dex,
    $combat_rolled_b_dex,
    $combat_might_b_str,
    $combat_might_a_end, // Defender.
    $combat_add_diff_b_str,
    $combat_rolled_b_str
  );
  array_unshift($attack_b_vs_a, 'B Attacks A:');
  $remove_field = array_pop($attack_b_vs_a);
  $remove_field = array_pop($attack_b_vs_a);
  $attack_b_vs_a[] = ($attack_b_vs_a['pp'] + ($attack_b_vs_a['pp'] * $combat_weapon_b)) . t(' to A, w/ weapon');
  $rows[] = $attack_b_vs_a;
  $attack_a_points = $attack_a_vs_b['pp']; // PP produced by a
  $attack_b_points = $attack_b_vs_a['pp']; // PP produced by b
  $block_a_vs_b = _dice_ruler_make_two_trait_ruling(
    $combat_might_a_str,
    $combat_might_b_dex, // Defender.
    $combat_add_diff_a_str,
    $combat_rolled_a_str,
    $combat_might_a_end,
    $combat_might_b_str, // Defender.
    $combat_add_diff_a_end,
    $combat_rolled_a_end
  );
  array_unshift($block_a_vs_b, 'A Blocks B:');
  $remove_field = array_pop($block_a_vs_b);
  $remove_field = array_pop($block_a_vs_b);
  $block_a_vs_b['attack'] = (($attack_b_points - $block_a_vs_b['pp']) > 0) ?
    ($attack_b_points - $block_a_vs_b['pp'] + (($attack_b_points - $block_a_vs_b['pp']) * $combat_weapon_b)) . t(' to A, w/ weapon') : 0 . t(' to A');
  $rows[] = $block_a_vs_b;
  $block_b_vs_a = _dice_ruler_make_two_trait_ruling(
    $combat_might_b_str,
    $combat_might_a_dex, // Defender.
    $combat_add_diff_b_str,
    $combat_rolled_b_str,
    $combat_might_b_end,
    $combat_might_a_str, // Defender.
    $combat_add_diff_b_end,
    $combat_rolled_b_end
  );
  array_unshift($block_b_vs_a, 'B Blocks A:');
  $remove_field = array_pop($block_b_vs_a);
  $remove_field = array_pop($block_b_vs_a);
  $block_b_vs_a['attack'] = (($attack_a_points - $block_b_vs_a['pp']) > 0) ?
    ($attack_a_points - $block_b_vs_a['pp'] + (($attack_a_points - $block_b_vs_a['pp']) * $combat_weapon_a)) . t(' to B, w/ weapon') : 0 . t(' to B');
  $rows[] = $block_b_vs_a;
  $parry_a_vs_b = _dice_ruler_make_two_trait_ruling(
    $combat_might_a_dex,
    $combat_might_b_dex, // Defender.
    $combat_add_diff_a_dex,
    $combat_rolled_a_dex,
    $combat_might_a_str,
    $combat_might_b_str, // Defender.
    $combat_add_diff_a_str,
    $combat_rolled_a_str
  );
  array_unshift($parry_a_vs_b, 'A Parrys B:');
  $remove_field = array_pop($parry_a_vs_b);
  $remove_field = array_pop($parry_a_vs_b);
  $parry_a_vs_b['attack'] = (($attack_b_points - $parry_a_vs_b['pp']) > 0) ?
    ($attack_b_points - $parry_a_vs_b['pp'] + (($attack_b_points - $parry_a_vs_b['pp']) * $combat_weapon_b)) . t(' to A, w/ weapon') : 0 . t(' to A');
  $rows[] = $parry_a_vs_b;
  $parry_b_vs_a = _dice_ruler_make_two_trait_ruling(
    $combat_might_b_dex,
    $combat_might_a_dex, // Defender.
    $combat_add_diff_b_dex,
    $combat_rolled_b_dex,
    $combat_might_b_str,
    $combat_might_a_str, // Defender.
    $combat_add_diff_b_str,
    $combat_rolled_b_str
  );
  array_unshift($parry_b_vs_a, 'B Parrys A:');
  $remove_field = array_pop($parry_b_vs_a);
  $remove_field = array_pop($parry_b_vs_a);
  $parry_b_vs_a['attack'] = (($attack_a_points - $parry_b_vs_a['pp']) > 0) ?
    ($attack_a_points - $parry_b_vs_a['pp'] + (($attack_a_points - $parry_b_vs_a['pp']) * $combat_weapon_a)) . t(' to B, w/ weapon') : 0 . t(' to B');
  $rows[] = $parry_b_vs_a;
  $dodge_a_vs_b = _dice_ruler_make_two_trait_ruling(
    $combat_might_a_dex,
    $combat_might_b_dex, // Defender.
    $combat_add_diff_a_dex,
    $combat_rolled_a_dex,
    $combat_might_a_end,
    $combat_might_b_str, // Defender.
    $combat_add_diff_a_end,
    $combat_rolled_a_end
  );
  array_unshift($dodge_a_vs_b, 'A Dodges B:');
  $remove_field = array_pop($dodge_a_vs_b);
  $remove_field = array_pop($dodge_a_vs_b);
  $dodge_a_vs_b['attack'] = (($attack_b_points - $dodge_a_vs_b['pp']) > 0) ?
    ($attack_b_points - $dodge_a_vs_b['pp'] + (($attack_b_points - $dodge_a_vs_b['pp']) * $combat_weapon_b)) . t(' to A, w/ weapon') : 0 . t(' to A');
  $rows[] = $dodge_a_vs_b;
  $dodge_b_vs_a = _dice_ruler_make_two_trait_ruling(
    $combat_might_b_dex,
    $combat_might_a_dex, // Defender.
    $combat_add_diff_b_dex,
    $combat_rolled_b_dex,
    $combat_might_b_end,
    $combat_might_a_str, // Defender.
    $combat_add_diff_b_end,
    $combat_rolled_b_end
  );
  array_unshift($dodge_b_vs_a, 'B Dodges A:');
  $remove_field = array_pop($dodge_b_vs_a);
  $remove_field = array_pop($dodge_b_vs_a);
  $dodge_b_vs_a['attack'] = (($attack_a_points - $dodge_b_vs_a['pp']) > 0) ?
    ($attack_a_points - $dodge_b_vs_a['pp'] + (($attack_a_points - $dodge_b_vs_a['pp']) * $combat_weapon_a)) . t(' to B, w/ weapon') : 0 . t(' to B');
  $rows[] = $dodge_b_vs_a;
  // Now put all these into a table to display.
  $header = array(
    t('Action'),
    t('Might'),
    t('Diff'),
    t('Rolled'),
    t('Roll needed'),
    t('Success'),
    t('Winsby'),
    t('Points'),
    t('1/7th PP'),
    t('After defense'),
  );
  $output = vaxia_dice_table(array(
    'header' => $header,
    'rows' => $rows,
  ));
  return $output;
}

function _dice_ruler_magic(
  $magic_might_a,
  $magic_diff_a,
  $magic_add_diff_a,
  $magic_rolled_a,
  $magic_might_b,
  $magic_diff_b,
  $magic_add_diff_b,
  $magic_rolled_b,
  $magic_target_stat_int,
  $magic_target_stat_spi,
  $magic_target_skill
) {
  // How this normally works.
  // Add the difficulty for the casting to the target stat.
  // And figure out if the target was affected...
  // Check that all are numeric.
  if (!empty($magic_might_a) && !is_numeric($magic_might_a)
    || !empty($magic_diff_a) && !is_numeric($magic_diff_a)
    || !empty($magic_add_diff_a) && !is_numeric($magic_add_diff_a)
    || !empty($magic_rolled_a) && !is_numeric($magic_rolled_a)) {
    return t('Not all values given were numeric. We need numbers for math.');
  }
  // Check our required values are set.
  if (empty($magic_might_a)
    || empty($magic_diff_a)
    || empty($magic_rolled_a)) {
    return t('Not all required values given, or values are zero.');
  }
  // Set default if empty.
  if (empty($magic_add_diff_a)) {
    $magic_add_diff_a = 0;
  }
  // Check that all are numeric.
  if (!empty($magic_might_b) && !is_numeric($magic_might_b)
    || !empty($magic_diff_b) && !is_numeric($magic_diff_b)
    || !empty($magic_add_diff_b) && !is_numeric($magic_add_diff_b)
    || !empty($magic_rolled_b) && !is_numeric($magic_rolled_b)) {
    return t('Not all values given were numeric. We need numbers for math.');
  }
  // Check our required values are set.
  if (empty($magic_might_b)
    || empty($magic_diff_b)
    || empty($magic_rolled_b)) {
    return t('Not all required values given, or values are zero.');
  }
  // Set default if empty.
  if (empty($magic_add_diff_b)) {
    $magic_add_diff_b = 0;
  }
  // Set defaults for the target.
  if (empty($magic_target_stat_int)) {
    $magic_target_stat_int = 0;
  }
  if (empty($magic_target_stat_spi)) {
    $magic_target_stat_spi = 0;
  }
  if (empty($magic_target_skill)) {
    $magic_target_skill = 0;
  }
  // Check that all are numeric.
  if (!empty($magic_target_stat_int) && !is_numeric($magic_target_stat_int)
    || !empty($magic_target_stat_spi) && !is_numeric($magic_target_stat_spi)
    || !empty($magic_target_skill) && !is_numeric($magic_target_skill)) {
    return t('Not all values given were numeric. We need numbers for math.');
  }
  // Set header.
  $header = array(
    t('Might'),
    t('Diff'),
    t('Rolled'),
    t('Roll needed'),
    t('Success'),
    t('Winsby'),
    t('Points'),
    t('1/7th PP'),
    t('% of diff'),
    t('Failed might'),
  );
  $output = '';
  // Calculate the rulings to succeed at the spell at all.
  $rows = array();
  $results = _dice_ruler_make_magic_ruling(
    $magic_might_a,
    $magic_diff_a,
    $magic_add_diff_a,
    $magic_rolled_a,
    $magic_might_b,
    $magic_diff_b,
    $magic_add_diff_b,
    $magic_rolled_b);
  $rows[] = $results;
  $output .= vaxia_dice_table(array(
    'header' => $header,
    'rows' => $rows,
  ));
  // If we have a resisting target calculate the ruling for resisted.
  if ($magic_target_stat_int != 0 || $magic_target_stat_spi != 0 || $magic_target_skill != 0) {
    $output .= '<div><b>' . t('Resisted roll results') . ':</b></div>';
    // Set the target diffs for resisted rolls.
    $magic_diff_a = $magic_diff_a + $magic_target_stat_int + $magic_target_skill;
    $magic_diff_b = $magic_diff_b + $magic_target_stat_spi + $magic_target_skill;
    $output .= '<div>' . t('With target\'s resistance, the base difficulty is') .': '. $magic_diff_a .  ' / ' . $magic_diff_b . '</div>';
    // Combine results.
    $rows = array();
    $results = _dice_ruler_make_magic_ruling(
      $magic_might_a,
      $magic_diff_a,
      $magic_add_diff_a,
      $magic_rolled_a,
      $magic_might_b,
      $magic_diff_b,
      $magic_add_diff_b,
      $magic_rolled_b);
    $rows[] = $results;
    $output .= vaxia_dice_table(array(
      'header' => $header,
      'rows' => $rows,
    ));
  }
  return $output;
}

function _dice_ruler_make_magic_ruling(
  $magic_might_a,
  $magic_diff_a,
  $magic_add_diff_a,
  $magic_rolled_a,
  $magic_might_b,
  $magic_diff_b,
  $magic_add_diff_b,
  $magic_rolled_b) {
  // Get results from our ruling function.
  $magic_a = _dice_ruler_make_ruling(
    $magic_might_a,
    $magic_diff_a,
    0,
    $magic_rolled_a);
  $magic_b = _dice_ruler_make_ruling(
    $magic_might_b,
    $magic_diff_b,
    0,
    $magic_rolled_b);
  // Figure out as per a two-trait.
  $versus_one = (($magic_rolled_a - $magic_a['needed']) >= 0) ? $magic_add_diff_a : 0; // 4 => Roll needed
  $versus_two = (($magic_rolled_b - $magic_b['needed']) >= 0) ? $magic_add_diff_b : 0; // 4 => Roll needed
  $winsby_one = $magic_a['rolled'] - $magic_a['needed'];
  $winsby_two = $magic_b['rolled'] - $magic_b['needed'];
  $magic_winsby = ($winsby_one / 2) + ($winsby_two / 2) + $versus_one + $versus_two; // 6 => Windsby
  $magic_success = ($magic_winsby >= 0) ? 'yes' : 'no';
  if ($magic_success == 'no') {
    $magic_success = ($magic_winsby < -50) ? 'mishap' : 'no';
  }
  $magic_points = ($magic_winsby >= 0) ? ($magic_might_a + $magic_might_b) / 2 * $magic_winsby / 100 : 0;
  $magic_perc_diff = 'n/a';
  if ($magic_success != 'yes') {
    $magic_diff_perc_a = $magic_might_a * ((tan(($magic_rolled_a - 50.5) / 49.5 * atan(0.06 * 50)) / 6 + 0.5) / (0.5 - tan(($magic_rolled_a - 50.5) / 49.5 * atan(0.06 * 50)) / 6));
    $magic_diff_perc_b = $magic_might_b * ((tan(($magic_rolled_b - 50.5) / 49.5 * atan(0.06 * 50)) / 6 + 0.5) / (0.5 - tan(($magic_rolled_b - 50.5) / 49.5 * atan(0.06 * 50)) / 6));
    $magic_perc_diff = (($magic_diff_perc_a / $magic_diff_a) + ($magic_diff_perc_b / $magic_diff_b)) / 2;
  }
  $magic_failed_might = 0;
  if ($magic_success != 'yes') {
    $magic_failed_might = round((($magic_diff_a + $magic_diff_b) / 2) * (1 - $magic_perc_diff));
  }
  return array(
    'might' => '<div>' . $magic_might_a . '</div><div style="border-top:1px solid;">' . $magic_might_b . '</div>', // Display the two.
    'diff' => '<div>' . $magic_diff_a . '</div><div style="border-top:1px solid;">' . $magic_diff_b . '</div>', // Display the two.
    'rolled' => '<div>' . $magic_rolled_a . '</div><div style="border-top:1px solid;">' . $magic_rolled_b . '</div>', // Display the two.
    'needed' => '<div>' . number_format($magic_a['needed'], 1) . '</div><div style="border-top:1px solid;">' . number_format($magic_b['needed'], 1) . '</div>', // Display the two.
    'success' => $magic_success,
    'winsby' => is_numeric($magic_winsby) ? number_format($magic_winsby) : $magic_winsby,
    'pp' => is_numeric($magic_points) ? number_format($magic_points) : $magic_points,
    'seven_pp' => is_numeric($magic_points) ? number_format($magic_points / 7) : $magic_points,
    'perc_diff' => is_numeric($magic_perc_diff) ? number_format($magic_perc_diff) : $magic_perc_diff,
    'fail_might' => $magic_failed_might,
  );
}

function _dice_ruler_make_ruling(
  $one_trait_might,
  $one_trait_diff,
  $one_trait_add_diff,
  $one_trait_rolled,
  $format_results = FALSE
) {
  // The ladder is an eyeball. This is a gemoetric solution. I hope you like trig.
  $roll_needed = 50.5 + atan(0.06 * (100 * ($one_trait_diff + $one_trait_add_diff) / ($one_trait_diff + $one_trait_add_diff + $one_trait_might) - 50)) * 49.5 / atan(0.06 * 50);
  $success = ($one_trait_rolled < $roll_needed) ? 'no' : 'yes';
  $winsby_diff = (($one_trait_rolled - $roll_needed) >= 0 ) ? $one_trait_add_diff : 0;
  $winsby = ($one_trait_rolled - $roll_needed ) + $winsby_diff;
  $points = 0;
  if ($success == 'yes') {
    $points = $one_trait_might * ( $winsby / 100);
  }
  $perc_diff = 'n/a';
  if ($success == 'no') {
    $diff_perc = $one_trait_might * ((tan(($one_trait_rolled - 50.5) / 49.5 * atan(0.06 * 50)) / 6 + 0.5) / (0.5 - tan(($one_trait_rolled - 50.5) / 49.5 * atan(0.06 * 50)) / 6));
    $perc_diff = 0;
    if ($one_trait_diff != 0) {
      $perc_diff = $diff_perc / $one_trait_diff;
    }
  }
  $failed_might = 0;
  if ($success == 'no') {
    $failed_might = round($one_trait_diff * (1 - $perc_diff));
  }
  // Return array results.
  return array(
    'might' => $one_trait_might,
    'diff' => $one_trait_diff,
    'rolled' => $one_trait_rolled,
    'needed' => ($format_results) ? number_format($roll_needed) : $roll_needed,
    'success' => $success,
    'winsby' => ($format_results) ? number_format($winsby) : $winsby,
    'pp' => ($format_results) ? number_format($points) : $points,
    'seven_pp' => ($format_results) ? number_format($points / 7) : $points,
    'perc_diff' => is_numeric($perc_diff) ? number_format($perc_diff) : $perc_diff,
    'fail_might' => $failed_might,
  );
}

function _dice_ruler_make_two_trait_ruling(
    $two_trait_might_a,
    $two_trait_diff_a,
    $two_trait_add_diff_a,
    $two_trait_rolled_a,
    $two_trait_might_b,
    $two_trait_diff_b,
    $two_trait_add_diff_b,
    $two_trait_rolled_b
) {
  // Get results from our master function.
  $trait_a = _dice_ruler_make_ruling(
    $two_trait_might_a,
    $two_trait_diff_a,
    $two_trait_add_diff_a,
    $two_trait_rolled_a);
  $trait_b = _dice_ruler_make_ruling(
    $two_trait_might_b,
    $two_trait_diff_b,
    $two_trait_add_diff_b,
    $two_trait_rolled_b);
  $versus_one = (($two_trait_rolled_a - $trait_a['needed']) >= 0) ? $two_trait_add_diff_a : 0; // 4 => Roll needed
  $versus_two = (($two_trait_rolled_b - $trait_b['needed']) >= 0) ? $two_trait_add_diff_b : 0; // 4 => Roll needed
  $winsby_one = $trait_a['rolled'] - $trait_a['needed'];
  $winsby_two = $trait_b['rolled'] - $trait_b['needed'];
  $two_trait_winsby = ($winsby_one / 2) + ($winsby_two / 2) + $versus_one + $versus_two; // 6 => Windsby
  $two_trait_success = ($two_trait_winsby >= 0) ? 'yes' : 'no';
  if ($two_trait_success == 'no') {
    $two_trait_success = ($two_trait_winsby < -50) ? 'mishap' : 'no';
  }
  $two_trait_points = ($two_trait_winsby >= 0) ? ($two_trait_might_a + $two_trait_might_b) / 2 * $two_trait_winsby / 100 : 0;
  $two_trait_perc_diff = 'n/a';
  if ($two_trait_success != 'yes') {
    $two_trait_diff_perc_a = $two_trait_might_a * ((tan(($two_trait_rolled_a - 50.5) / 49.5 * atan(0.06 * 50)) / 6 + 0.5) / (0.5 - tan(($two_trait_rolled_a - 50.5) / 49.5 * atan(0.06 * 50)) / 6));
    $two_trait_diff_perc_b = $two_trait_might_b * ((tan(($two_trait_rolled_b - 50.5) / 49.5 * atan(0.06 * 50)) / 6 + 0.5) / (0.5 - tan(($two_trait_rolled_b - 50.5) / 49.5 * atan(0.06 * 50)) / 6));
    $two_trait_perc_diff = (($two_trait_diff_perc_a / $two_trait_diff_a) + ($two_trait_diff_perc_b / $two_trait_diff_b)) / 2;
  }
  $two_trait_failed_might = 0;
  if ($two_trait_success != 'yes') {
    $two_trait_failed_might = round((($two_trait_diff_a + $two_trait_diff_b) / 2) * (1 - $two_trait_perc_diff));
  }
  // Combine results.
  return array(
    'might' => '<div>' . $two_trait_might_a . '</div><div style="border-top:1px solid;">' . $two_trait_might_b . '</div>', // Display the two.
    'diff' => '<div>' . $two_trait_diff_a . '</div><div style="border-top:1px solid;">' . $two_trait_diff_b . '</div>', // Display the two.
    'rolled' => '<div>' . $two_trait_rolled_a . '</div><div style="border-top:1px solid;">' . $two_trait_rolled_b . '</div>', // Display the two.
    'needed' => '<div>' . number_format($trait_a['needed'], 1) . '</div><div style="border-top:1px solid;">' . number_format($trait_b['needed'], 1) . '</div>', // Display the two.
    'success' => $two_trait_success,
    'winsby' => is_numeric($two_trait_winsby) ? number_format($two_trait_winsby) : $two_trait_winsby,
    'pp' => is_numeric($two_trait_points) ? number_format($two_trait_points) : $two_trait_points,
    'seven_pp' => is_numeric($two_trait_points) ? number_format($two_trait_points / 7) : $two_trait_points,
    'perc_diff' => is_numeric($two_trait_perc_diff) ? number_format($two_trait_perc_diff) : $two_trait_perc_diff,
    'fail_might' => $two_trait_failed_might,
  );
}

function _dice_ruler_read_dice($dice_rolls, $difficulty = 0, $bonus = 0) {
  // Create a formal result set for dice.
  $roll_ruling = array();
  // Read on all results.
  $roll_ruling['commands'] = array();
  $roll_ruling['results'] = array();
  $roll_ruling['mights'] = $roll_ruling['rolls'] = array();
  $roll_ruling['skill'] = $roll_ruling['stat'] = $roll_ruling['stat_name'] = array();
  $roll_ruling['aspects'] = $roll_ruling['notes'] = array();
  $roll_ruling['diff'] = $difficulty;
  $roll_ruling['bonus'] = $bonus;
  $roll_ruling['1s'] = $roll_ruling['100s'] = 0;
  $roll_ruling['by'] = $roll_ruling['PPs'] = 0;
  $roll_ruling['crit_fails'] = $roll_ruling['crit_wins'] = 0;
  $roll_ruling['success'] = FALSE;
  // Figure out the dice rolls from here.
  if (!empty($dice_rolls)) {
    // Break all dice up looking for the rolls that got with them.
    foreach ($dice_rolls as $index => $dice_roll) {
      // Set values.
      $aspects = array();
      $stat_name = '';
      $stat = $skill = $might = 0;
      $commands = isset($dice_rolls[$index]['roll_command']) ? $dice_rolls[$index]['roll_command'] : '';
      $results = isset($dice_rolls[$index]['roll_result']) ? $dice_rolls[$index]['roll_result'] : '';
      $notes = isset($dice_rolls[$index]['roll_notes']) ? $dice_rolls[$index]['roll_notes'] : '';
      // Old style.
      if (strpos($notes, '<br>') !== FALSE) {
        $notes = explode('<br>', $notes);
        $skip = array_shift($notes);
        $notes = isset($notes[$index]) ? $notes[$index] : '';
      }
      $note = $notes;
      $note = str_replace('1 : roll(1d100)', '1d100', $note);
      // Look for the 'title' alt for notes with skills, and get the data out of them if we can.
      if (strpos($note, '<a title="') !== FALSE) {
        // New style.
        if (strpos($note, 'aspects="') !== FALSE && preg_match('/aspects="(.+?)"/', $note, $matches)) {
          if (!empty($matches[1])) {
            $found = explode(' ', $matches[1]);
            $aspects = array_merge($aspects, $found);
          }
        }
        if (strpos($note, 'stat_name="') !== FALSE && preg_match('/stat_name="(.+?)"/', $note, $matches)) {
          if (!empty($matches[1])) {
            $stat_name = $matches[1];
          }
        }
        if (strpos($note, 'stat="') !== FALSE && preg_match('/stat="(.+?)"/', $note, $matches)) {
          if (!empty($matches[1]) && $stat_name != 'any') {
            $stat = (int) $matches[1];
          }
        }
        if (strpos($note, 'skill="') !== FALSE && preg_match('/skill="(.+?)"/', $note, $matches)) {
          if (!empty($matches[1])) {
            $skill = (int) $matches[1];
          }
        }
        // Check old style.
        if (empty($aspects) && preg_match('/title=".+?\((.+?)\)"/', $note, $matches)) {
          if (!empty($matches[1])) {
            $found = explode(', ', $matches[1]);
            $aspects = array_merge($aspects, $found);
          }
        }
        if (empty($skill) && preg_match('/title=".+?".+?\((.+?)\)/', $note, $matches)) {
          if (!empty($matches[1])) {
            $skill = $matches[1];
          }
        }
        // Trim any aspects.
        foreach ($aspects as $a_i => $aspect) {
          $aspects[$a_i] = trim($aspect);
        }
      } // End checking for stats with skills.
      // Check for stat information new style.
      if (empty($stat_name) && preg_match('/(...) \(.+?\)/', $note, $matches)) {
        if (!empty($matches[1])) {
          $stat_name = trim($matches[1]);
        }
      }
      if (empty($stat) && preg_match('/... \((.+?)\)/', $note, $matches)) {
        if (!empty($matches[1])) {
          $stat = (int) $matches[1];
        }
      }
      if (empty($stat_name) && preg_match('/d100 (...) \(.+?\)/', $note, $matches)) {
        if (!empty($matches[1])) {
          $stat_name = trim($matches[1]);
        }
      }
      if (empty($stat) && preg_match('/d100 ... \((.+?)\)/', $note, $matches)) {
        if (!empty($matches[1])) {
          $stat = (int) $matches[1];
        }
      }
      // Old style.
      if (empty($stat_name) && preg_match('/d100 . (...) \(.+?\)/', $note, $matches)) {
        if (!empty($matches[1])) {
          $stat_name = trim($matches[1]);
        }
      }
      if (empty($stat) && preg_match('/d100 . ... \((.+?)\)/', $note, $matches)) {
        if (!empty($matches[1])) {
          $stat = (int) $matches[1];
        }
      }
      if (empty($stat_name) && preg_match('/d100  . (...) \(.+?\)/', $note, $matches)) {
        if (!empty($matches[1])) {
          $stat_name = trim($matches[1]);
        }
      }
      if (empty($stat) && preg_match('/d100  . ... \((.+?)\)/', $note, $matches)) {
        if (!empty($matches[1])) {
          $stat = (int) $matches[1];
        }
      }
      // Lastly, get the roll.
      $roll_results = explode('=', $results);
      $roll_results = explode(',', $roll_results[0]);
      $might = 0;
      if (count($roll_results) > 1 && (!empty($stat) || !empty($skill))) {
        // Get the might off the roll.
        $might = trim(array_pop($roll_results));
      }
      // Extract the aspects from the nodes.
      //if (!empty($stat) || !empty($skill)) {
      //foreach ($roll_results as $roll_result) {
      //  $roll_ruling['commands'][] = $commands;
      //  $roll_ruling['results'][] = $results;
      //  $roll_ruling['aspects'][] = $aspects;
      //  $roll_ruling['stat_name'][] = ($stat_name != 'any') ? $stat_name : '';
      //  $roll_ruling['stat'][] = $stat;
      //  $roll_ruling['skill'][] = $skill;
      //  $roll_ruling['mights'][] = $stat + $skill;
      //  $roll_ruling['rolls'][] = trim(implode(', ', $roll_results));
      //  $roll_ruling['notes'][] = $note;
      //} // End roll processing.
      //}
      //else {
        $roll_ruling['commands'][] = $commands;
        $roll_ruling['results'][] = $results;
        $roll_ruling['aspects'][] = $aspects;
        $roll_ruling['stat_name'][] = ($stat_name != 'any') ? $stat_name : '';
        $roll_ruling['stat'][] = $stat;
        $roll_ruling['skill'][] = $skill;
        $roll_ruling['mights'][] = !empty($might) ? $might : ($stat + $skill);
        $roll_ruling['rolls'][] = trim(implode(', ', $roll_results));
        $roll_ruling['notes'][] = $note;
      //}
    } // End all roles.

    // If difficulty is set, then we process all rolls
    // to date in light of their available settings and stuff.
    // The following values are after processing for the entire set.
    if ($difficulty > 0) {
      // Figure out if this is a 1 trait roll.
      if (count($roll_ruling['mights']) == 1) {
        $result = _dice_ruler_make_ruling(
          $roll_ruling['mights'][0] + $roll_ruling['bonus'],
          $roll_ruling['diff'],
          0,
          $roll_ruling['rolls'][0]
        );
        if ($roll_ruling['rolls'][0] == 1) {
          $roll_ruling['1s']++;
        }
        if ($roll_ruling['rolls'][0] == 100) {
          $roll_ruling['100s']++;
        }
      }
      // Figure out if this is a 2 trait roll.
      if (count($roll_ruling['mights']) > 1) {
        $result = _dice_ruler_make_two_trait_ruling(
          $roll_ruling['mights'][0] + $roll_ruling['bonus'],
          $roll_ruling['diff'],
          0,
          $roll_ruling['rolls'][0],
          $roll_ruling['mights'][1] + $roll_ruling['bonus'],
          $roll_ruling['diff'],
          0,
          $roll_ruling['rolls'][1]
        );
        if ($roll_ruling['rolls'][0] == 1) {
          $roll_ruling['1s']++;
        }
        if ($roll_ruling['rolls'][0] == 100) {
          $roll_ruling['100s']++;
        }
        if ($roll_ruling['rolls'][1] == 1) {
          $roll_ruling['1s']++;
        }
        if ($roll_ruling['rolls'][1] == 100) {
          $roll_ruling['100s']++;
        }
      }
      // Now, if we have a ruling, let's get some info out of it too.
      if (!empty($result)) {
        $by_generated = $result['winsby'];
        $PP_generated = $result['pp'];
        if ($by_generated < 0) {
          $PP_generated = floor( $PP_generated * (($by_generated * -1) / 100) );
          $roll_ruling['success'] = FALSE;
          if ($by_generated <= -25) {
            $roll_ruling['crit_fails']++;
          }
        }
        if ($by_generated >= 0) {
          $roll_ruling['success'] = TRUE;
          if ($by_generated >= 25) {
            $roll_ruling['crit_wins']++;
          }
        }
        $roll_ruling['PPs'] += $PP_generated;
        $roll_ruling['by'] += $by_generated;
      }
    }
    // Ok, we've now analyzed everything for this ruling, time to return it.
    // So the calling system can make use of it as needed.
  }
  return $roll_ruling;
}
