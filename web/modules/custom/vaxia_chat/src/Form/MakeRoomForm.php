<?php

namespace Drupal\vaxia_chat\Form;

use Drupal\Core\Form\FormBase;
use Drupal\Core\Form\FormStateInterface;
use Drupal\node\Entity\Node;
use Drupal\node\NodeInterface;

/**
 * "Make a Room from this article" on wiki pages (D7 room_cleanup): a room
 * named after the article's last title part ("Docks/Fish Market"), under the
 * room named like the part before it, linked back to the article.
 */
class MakeRoomForm extends FormBase {

  public function getFormId() {
    return 'room_cleanup_make_room_from_wiki';
  }

  public function buildForm(array $form, FormStateInterface $form_state, ?NodeInterface $wiki = NULL) {
    $form_state->set('wiki_nid', (int) $wiki->id());
    $form['room_cleanup_make_room_submit'] = ['#type' => 'submit', '#value' => $this->t('Make a Room from this article')];
    return $form;
  }

  public function submitForm(array &$form, FormStateInterface $form_state) {
    $wiki = Node::load($form_state->get('wiki_nid'));
    if (!$wiki) {
      $this->messenger()->addStatus($this->t('Unable to identify wiki article to create this chatroom.'));
      return;
    }
    $parts = explode('/', $wiki->label());
    $room = Node::create([
      'type' => 'rpg_chatroom',
      'title' => trim(array_pop($parts)),
      'uid' => $this->currentUser()->id(),
      'status' => 1,
    ]);
    if ($wiki->hasField('field_tag_realm') && $room->hasField('field_tag_realm')) {
      $room->set('field_tag_realm', $wiki->get('field_tag_realm')->getValue());
    }
    $parent_title = trim((string) array_pop($parts));
    if ($parent_title !== '') {
      $parents = \Drupal::entityTypeManager()->getStorage('node')->loadByProperties(['title' => $parent_title]);
      if ($parent = end($parents)) {
        $this->messenger()->addStatus($this->t('Found suggested parent room :title.', [':title' => $parent->label()]));
        $room->set('field_parent', $parent->id());
      }
    }
    $room->set('field_wiki_source', $wiki->id());
    $room->save();
    $form_state->setRedirect('entity.node.edit_form', ['node' => $room->id()]);
  }

}
