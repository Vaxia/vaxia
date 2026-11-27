/**
 * @file
 * Vaxia theme behaviours (D7 vaxia/js/vaxia.js): alternating classes on wiki
 * article images and paragraphs, and button styling for node links.
 *
 * Not carried over: the Colorbox image wrap (no Colorbox on D11) and the
 * Pinterest pin display (vaxia_pins.js; Pinterest's embed script is gone).
 */
(function (Drupal, once) {
  Drupal.behaviors.themeVaxia = {
    attach(context) {
      once('vaxia-even-odd', 'article .field--name-body img, article .field--name-body p', context).forEach((element) => {
        const siblings = element.parentElement ? Array.from(element.parentElement.querySelectorAll(':scope > ' + element.tagName.toLowerCase())) : [];
        element.classList.add(siblings.indexOf(element) % 2 ? 'odd' : 'even');
      });
      once('vaxia-buttons', '#content .links a', context).forEach((link) => {
        link.classList.add('button', 'button-small');
      });
    },
  };
})(Drupal, once);
