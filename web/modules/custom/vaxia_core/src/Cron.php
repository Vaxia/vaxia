<?php

namespace Drupal\vaxia_core;

/**
 * Once-a-day cron jobs (D7 _vaxia_once_a_day_cron()). The last run is kept
 * in state "vaxia_cron.<job>" and set to <time> today, so runs don't drift.
 */
class Cron {

  public static function onceADay(string $job, string $time = '01:00:00'): bool {
    $state = \Drupal::state();
    $last = (int) $state->get('vaxia_cron.' . $job, strtotime('-10 years'));
    if ($last >= strtotime('-1 day')) {
      return FALSE;
    }
    \Drupal::logger('vaxia_cron')->notice('Calling @job at @time', ['@job' => $job, '@time' => $time]);
    $state->set('vaxia_cron.' . $job, strtotime(date('m/d/Y ' . $time)));
    return TRUE;
  }

  /**
   * Workflow states as "Workflow: state" options (D7 _vaxia_get_workflow_options()).
   */
  public static function stateOptions(): array {
    $options = [];
    foreach (\Drupal\workflow\Entity\WorkflowState::loadMultiple() as $sid => $state) {
      if (!$state->isCreationState()) {
        $options[$sid] = $state->getWorkflow()->label() . ': ' . $state->label();
      }
    }
    return $options;
  }

}
