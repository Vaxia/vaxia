<?php

namespace Drupal\vaxia_chat\Form;

use Drupal\comment\Entity\Comment;
use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\Entity\Node;
use Drupal\node\NodeInterface;

/**
 * Posts "In <realm>, <suggestion> at <room>." as a narrative bot post: a
 * random play suggestion and a random active public room of its realm that
 * isn't mature or locked (D7 _play_suggestions_search()).
 */
class PlaySuggestionForm extends FormBase {

  public function getFormId() {
    return '_play_suggestions_search';
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?NodeInterface $node = NULL) {
    $form_state->set('nid', (int) $node->id());
    $form['lets_play'] = ['#type' => 'submit', '#value' => $this->t("I'm bored.")];
    return $form;
  }

  /**
   * One post per click (D7 also posted from validation: known bug, two posts).
   */
  public function submitForm(array &$form, FormStateInterface $form_state) {
    $db = \Drupal::database();
    $suggestions = $db->query("SELECT n.nid, n.title, t.field_tag_realm_target_id AS tid FROM {node_field_data} n LEFT JOIN {node__field_tag_realm} t ON t.entity_id = n.nid WHERE n.type = 'play_suggestions'")->fetchAll();
    if (!$suggestions) {
      return;
    }
    $suggestion = $suggestions[array_rand($suggestions)];
    $realm = $suggestion->tid ? \Drupal\taxonomy\Entity\Term::load($suggestion->tid) : NULL;
    if (!$realm) {
      return;
    }
    // Rooms with posts: public ("1" shows in power search), not mature, not
    // locked, in the suggestion's realm.
    $rooms = $db->query("SELECT DISTINCT n.nid FROM {comment_field_data} c INNER JOIN {node_field_data} n ON n.nid = c.entity_id AND c.entity_type = 'node'
      INNER JOIN {node__field_tag_realm} r ON r.entity_id = n.nid
      INNER JOIN {node__field_private_room} p ON p.entity_id = n.nid
      LEFT JOIN {node__field_mature_posting} m ON m.entity_id = n.nid
      LEFT JOIN {node__field_locked_to_users} l ON l.entity_id = n.nid
      WHERE n.type = 'rpg_chatroom' AND n.status = 1 AND r.field_tag_realm_target_id = :realm AND p.field_private_room_value = 1
      AND COALESCE(m.field_mature_posting_value, 0) = 0 AND COALESCE(l.field_locked_to_users_value, 0) = 0
      ORDER BY n.nid", [':realm' => $realm->id()])->fetchCol();
    if (!$rooms) {
      return;
    }
    $room = Node::load($rooms[array_rand($rooms)]);
    $location = $room->toLink()->toString();
    $field = NULL;
    $target = Node::load($form_state->get('nid'));
    foreach ($target->getFieldDefinitions() as $name => $definition) {
      if ($definition->getType() === 'comment') {
        $field = $name;
      }
    }
    if (!$field) {
      return;
    }
    Comment::create([
      'entity_type' => 'node',
      'entity_id' => $target->id(),
      'field_name' => $field,
      'comment_type' => $target->getFieldDefinition($field)->getSetting('comment_type'),
      'uid' => 1,
      'subject' => 'Play suggestion',
      'status' => 1,
      'comment_body' => ['value' => 'In ' . $realm->label() . ', ' . $suggestion->title . ' at ' . $location . '.', 'format' => 'wiki'],
      'field_ooc' => ['value' => (string) $this->t('Want to add some suggestions? [url=/node/add/play_suggestions]Click here.[/url]'), 'format' => 'wiki'],
      'field_comment_narrative' => 1,
    ])->save();
  }

}
