<?php

namespace Vaxia\Tests;

/**
 * Files strikes through the strike form, as social staff do.
 */
trait StrikeFormTrait {

  /** @var object|null Social staff member who files strikes. */
  private $strikeFiler;

  protected function strikeFiler() {
    if (!$this->strikeFiler) {
      $this->strikeFiler = $this->site->createUser(array('lead'), array('create strike content', 'edit any notation content'));
    }
    return $this->strikeFiler;
  }

  /**
   * Submits node/add/strike. Leaves the result page loaded (messages, etc.).
   *
   * @param string $severity
   *   'Yellow' or 'Red'.
   * @param string $when
   *   Date of incident, anything strtotime() reads.
   */
  protected function fileStrike($player, $severity, $when = 'now', $notation = NULL) {
    $ts = strtotime($when);
    // The form requires an associated record (the notation behind the strike).
    $notation = $notation ?: $this->site->createNotation($player);
    $this->login($this->strikeFiler());
    $this->get('node/add/strike');
    $edit = array(
      'title' => $severity . ' strike - ' . $player->name,
      'field_severity[und]' => $severity,
      'field_struck_player[und][0][uid]' => $this->referenceValue($player->name, $player->uid, 'user'),
    ) + $this->dateEdit('field_date_of_incident', $ts);
    $edit['field_associated_record[und][0][nid]'] = $this->referenceValue($notation->title, $notation->nid);
    $this->submitForm(NULL, $edit, 'Save');
    $this->assertSame(array(), array_filter($this->messages('error'), function ($m) {
      return strpos($m, 'Notice') === FALSE && strpos($m, 'Warning') === FALSE;
    }), 'Strike form accepted');
  }

}
