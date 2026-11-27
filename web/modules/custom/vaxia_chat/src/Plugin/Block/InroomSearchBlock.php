<?php

namespace Drupal\vaxia_chat\Plugin\Block;

use Drupal\Core\Block\Attribute\Block;
use Drupal\Core\StringTranslation\TranslatableMarkup;
use Drupal\Core\Url;

/**
 * Room search by name (required) for use inside rooms, with a link to Power
 * Search (D7 view rpg_chats, display inroom_rpg_chats).
 */
#[Block(id: 'rpg_chats-inroom_rpg_chats', admin_label: new TranslatableMarkup('In-Room Search RPG Chats'))]
class InroomSearchBlock extends RoomSearchBlock {

  const DISPLAY = 'inroom_rpg_chats';
  const NAME_KEY = 'inroom';
  const REALM_KEY = NULL;
  const BUTTON = 'Go';

  /**
   * The name is required: no rooms until one is given.
   */
  protected function searched(string $name): bool {
    return $name !== '';
  }

  protected function emptyText(bool $searched): ?string {
    return (string) $this->t('No rooms found matching this name, or no search performed.');
  }

  protected function footer(): ?string {
    // D7 linked /power-search-archive, production's alias of the site search.
    return (string) $this->t('Or use the <a href=":url">Power Search</a>', [':url' => Url::fromRoute('vaxia_core.power_search')->toString()]);
  }

}
