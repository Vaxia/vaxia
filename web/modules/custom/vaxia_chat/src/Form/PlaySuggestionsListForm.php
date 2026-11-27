<?php

namespace Drupal\vaxia_chat\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\Entity\Node;

/**
 * Staff list of play suggestions, newest first, 35 a page, with D7's bulk
 * "Delete item" operation (D7 view play_suggestions, a VBO table).
 */
class PlaySuggestionsListForm extends FormBase {

  public function getFormId() {
    return 'vaxia_chat_play_suggestions_list';
  }

  public function buildForm(array $form, FormStateInterface $form_state) {
    $storage = \Drupal::entityTypeManager()->getStorage('node');
    $ids = $storage->getQuery()->accessCheck(FALSE)->condition('type', 'play_suggestions')->condition('status', 1)->sort('created', 'DESC')->sort('nid', 'DESC')->pager(35)->execute();
    $options = [];
    foreach ($storage->loadMultiple($ids) as $nid => $node) {
      $realms = [];
      foreach ($node->get('field_tag_realm')->referencedEntities() as $term) {
        $realms[] = $term->toLink()->toString();
      }
      $options[$nid] = [
        'realm' => ['data' => ['#markup' => implode(', ', $realms)]],
        'title' => ['data' => $node->toLink()->toRenderable()],
      ];
    }
    $form['actions'] = ['#type' => 'actions', 'delete' => ['#type' => 'submit', '#value' => $this->t('Delete item')]];
    $form['nodes'] = [
      '#type' => 'tableselect',
      '#header' => ['realm' => $this->t('Realm'), 'title' => $this->t('Title')],
      '#options' => $options,
      '#empty' => '',
    ];
    $form['pager'] = ['#type' => 'pager'];
    $form['#cache'] = ['contexts' => ['url.query_args.pagination:page', 'user.roles'], 'tags' => ['node_list', 'taxonomy_term_list']];
    return $form;
  }

  public function validateForm(array &$form, FormStateInterface $form_state) {
    if (!array_filter($form_state->getValue('nodes'))) {
      $form_state->setErrorByName('nodes', $this->t('Please select at least one item.'));
    }
  }

  /**
   * Hands the selection to core's "Delete content" confirmation.
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $nodes = array_filter(Node::loadMultiple(array_filter($form_state->getValue('nodes'))), fn($node) => $node->access('delete'));
    if (!$nodes) {
      return;
    }
    \Drupal::service('plugin.manager.action')->createInstance('entity:delete_action:node')->executeMultiple($nodes);
    $form_state->setRedirect('entity.node.delete_multiple_form', [], ['query' => ['destination' => '/play-suggestions']]);
  }

}
