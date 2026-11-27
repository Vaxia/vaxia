<?php

namespace Drupal\vaxia_chat;

use Drupal\comment\Entity\Comment;
use Drupal\node\Entity\Node;

/**
 * Moves old chat and forum posts to flat HTML files (one page per room per
 * day) with character, location and player indexes, and searches them (D7
 * archives module).
 *
 * The D7 search ran grep through the shell with the search text in the
 * command (known bug: command injection). This one reads the files in PHP.
 */
class Archives {

  public static function root(): string {
    return (string) \Drupal::config('vaxia_chat.settings')->get('archives_flat_file_path');
  }

  /**
   * Archives every chat and forum post older than the configured timeframe.
   */
  public static function archiveAll(): void {
    $messenger = \Drupal::messenger();
    $oldest = \Drupal::database()->query("SELECT c.created FROM {comment_field_data} c INNER JOIN {node_field_data} n ON c.entity_id = n.nid AND c.entity_type = 'node' WHERE n.type IN ('rpg_chatroom', 'forum') ORDER BY c.created ASC", [])->fetchField();
    $begin = time();
    if ($oldest) {
      $begin = strtotime(date('m/d/Y 00:00:01', strtotime('-1 day', (int) $oldest)));
    }
    $messenger->addStatus('Archiving comments, started looking from: ' . date('m/d/Y 00:00:01', $begin) . '.');
    $now = strtotime(\Drupal::config('vaxia_chat.settings')->get('archives_date') . ' ago');
    while ($begin < $now) {
      $end = strtotime('+1 day', $begin);
      self::writeFiles(self::collect('rpg_chatroom', 'room', $begin, $end), $begin, $end);
      self::writeFiles(self::collect('forum', 'forum', $begin, $end, TRUE), $begin, $end);
      clearstatcache();
      $begin = $end;
    }
    $messenger->addStatus('Archive complete.');
    \Drupal::state()->set('vaxia_chat.archives_last_run', date('m/d/Y H:i:s'));
  }

  /**
   * One day's posts of one content type as pages and index lines.
   */
  public static function collect(string $type, string $category, int $begin, int $end, bool $include_node = FALSE): array {
    $rows = \Drupal::database()->query("SELECT c.cid, c.entity_id FROM {comment_field_data} c INNER JOIN {node_field_data} n ON c.entity_id = n.nid AND c.entity_type = 'node' WHERE n.type = :type AND c.created BETWEEN :begin AND :end ORDER BY c.entity_id ASC, c.created ASC", [':type' => $type, ':begin' => $begin, ':end' => $end])->fetchAll();
    $renderer = \Drupal::service('renderer');
    $view = \Drupal::entityTypeManager()->getViewBuilder('comment');
    $pages = [];
    $path = date('Y/m/d', $begin) . '/';
    foreach ($rows as $row) {
      $comment = Comment::load($row->cid);
      $node = Node::load($row->entity_id);
      if (!$comment || !$node) {
        \Drupal::messenger()->addStatus('Missing ' . (!$node ? 'node #' . $row->entity_id : 'comment #' . $row->cid) . ' for processing.');
        continue;
      }
      $build = $view->view($comment);
      unset($build['links']);
      $display = (string) $renderer->renderInIsolation($build);
      $character = $comment->hasField('field_comment_character') && $comment->get('field_comment_character')->entity ? trim($comment->get('field_comment_character')->entity->label()) : '';
      $location = self::prepare($node->label());
      $player = trim((string) $comment->getOwner()->getAccountName());
      $realm = 'storyhost';
      if ($node->hasField('field_tag_realm') && $node->get('field_tag_realm')->entity) {
        $realm = strtolower($node->get('field_tag_realm')->entity->label());
      }
      $parent = '';
      if ($node->hasField('field_parent') && $node->get('field_parent')->entity) {
        $parent = self::prepare($node->get('field_parent')->entity->label());
        $parent = $parent !== '' ? $parent . '-' : '';
      }
      $page = $path . $category . '/' . strtolower($realm . '-' . $parent . $location) . '.html';
      if (empty($pages['pages'][$page])) {
        $pages['pages'][$page] = "\n<html><body style=\"background:white;\"> <style> .post{border:solid 1px #000;margin:0.25em;padding:0.25em;}.post header{width:150px;float:left;}.post header img{display:none;}.post .comment-spacer{clear:both;}.post .field-name-comment-body{margin-left:155px;}.post .dice_rolls{margin-left:155px;}</style>";
        if ($include_node) {
          $node_build = \Drupal::entityTypeManager()->getViewBuilder('node')->view($node);
          unset($node_build['links']);
          $pages['pages'][$page] .= "\n<div class=\"post\">\n" . $renderer->renderInIsolation($node_build) . "\n</div>";
          $pages['delete_nodes'][] = (int) $node->id();
        }
      }
      $pages['pages'][$page] .= "\n<div class=\"post\">\n" . $display . "\n</div>";
      if ($character !== '') {
        $pages['d']['character'][$character][] = '"' . $page . '",';
      }
      $pages['d']['location'][$location][] = '"' . $page . '",';
      $pages['d']['player'][$player][] = '"' . $page . '",';
      $pages['delete_comments'][] = (int) $comment->id();
    }
    foreach ($pages['pages'] ?? [] as $page => $html) {
      $pages['pages'][$page] .= "\n</body></html>";
    }
    foreach ($pages['d'] ?? [] as $index => $entries) {
      foreach ($entries as $key => $lines) {
        $pages['d'][$index][$key] = "\n" . implode("\n", array_unique($lines));
      }
    }
    return $pages;
  }

  /**
   * Writes a day's pages and index lines, then removes the archived posts.
   */
  public static function writeFiles(array $pages, int $begin, int $end): void {
    if (empty($pages['pages'])) {
      return;
    }
    $messenger = \Drupal::messenger();
    $base = DRUPAL_ROOT . '/' . self::root();
    $ok = TRUE;
    foreach ($pages['pages'] as $page => $html) {
      $ok = self::write($base . $page, $html, FALSE) && $ok;
    }
    foreach ($pages['d'] ?? [] as $index => $entries) {
      foreach ($entries as $key => $lines) {
        if ($key !== '' && $lines !== '') {
          $ok = self::write($base . 'd/' . $index . '.' . $key . '.data', $lines, TRUE) && $ok;
        }
      }
    }
    $messenger->addStatus('Archiving comments from: ' . date('m/d/Y 00:00:01', $begin) . ' to: ' . date('m/d/Y 00:00:01', $end) . '.');
    if (!$ok) {
      \Drupal::logger('archives')->critical('Error writing archive files.');
      $messenger->addError('Error while archiving comments: files could not be written.');
      return;
    }
    $storage = \Drupal::entityTypeManager()->getStorage('comment');
    foreach ($pages['delete_comments'] as $cid) {
      if ($comment = $storage->load($cid)) {
        $comment->delete();
      }
    }
    $messenger->addStatus('Archived ' . count($pages['delete_comments']) . ' comments.');
    \Drupal::logger('archives')->notice('Archived @count comments.', ['@count' => count($pages['delete_comments'])]);
    if (!empty($pages['delete_nodes'])) {
      foreach (array_unique($pages['delete_nodes']) as $nid) {
        $left = \Drupal::database()->query("SELECT COUNT(*) FROM {comment_field_data} WHERE entity_type = 'node' AND entity_id = :nid AND status = 1", [':nid' => $nid])->fetchField();
        if (!$left && ($node = Node::load($nid))) {
          $node->delete();
        }
      }
      $messenger->addStatus('Archived ' . count($pages['delete_nodes']) . ' nodes.');
    }
  }

  private static function write(string $file, string $content, bool $append): bool {
    if (!is_dir(dirname($file)) && !@mkdir(dirname($file), 0755, TRUE)) {
      return FALSE;
    }
    return file_put_contents($file, $content, $append ? FILE_APPEND : 0) !== FALSE;
  }

  /**
   * Room titles as file names (D7 _archives_prepare_string()).
   */
  public static function prepare(string $string): string {
    $string = str_replace(['.', ',', '_', ':', '/', '\\', '?', '"', ')', '(', '*', '&', '^', '%$', '#', '@', '!', '~', '[', ']', '{', '}'], '', trim($string));
    return trim(str_replace(' ', '-', $string));
  }

  /**
   * Archive pages containing the text (case-insensitive), as links. Only
   * reads files under the archive root; the indexes (d/) don't count.
   */
  public static function search(string $text): string {
    $root = self::root();
    $text = trim(strip_tags($text));
    $base = DRUPAL_ROOT . '/' . $root;
    $found = [];
    if ($text !== '' && $root !== '' && is_dir($base)) {
      $files = new \RecursiveIteratorIterator(new \RecursiveDirectoryIterator($base, \FilesystemIterator::SKIP_DOTS));
      foreach ($files as $file) {
        $relative = substr($file->getPathname(), strlen($base));
        if (!$file->isFile() || strpos($relative, 'd/') === 0) {
          continue;
        }
        if (stripos((string) file_get_contents($file->getPathname()), $text) !== FALSE) {
          $found[] = $root . $relative;
        }
      }
    }
    if (!$found) {
      return '<ul><li>' . t('No results found.') . '</li></ul>';
    }
    sort($found);
    $base_url = \Drupal::request()->getSchemeAndHttpHost() . \Drupal::request()->getBasePath();
    $links = array_map(fn($p) => '<a href="' . htmlspecialchars($base_url . '/' . $p, ENT_QUOTES) . '" target="_blank">' . htmlspecialchars($p) . '</a>', $found);
    return '<ul><li>' . implode("</li>\n<li>", $links) . '</li></ul>';
  }

}
