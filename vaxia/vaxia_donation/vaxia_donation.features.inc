<?php
/**
 * @file
 * vaxia_donation.features.inc
 */

/**
 * Implements hook_views_api().
 */
function vaxia_donation_views_api($module = NULL, $api = NULL) {
  return array("api" => "3.0");
}

/**
 * Implements hook_default_payment_method().
 */
function vaxia_donation_default_payment_method() {
  $items = array();
  $items['paypal_vaxia_fund'] = entity_import('payment_method', '{
    "controller_data" : {
      "capture" : "0",
      "email_address" : "vaxiauserone@gmail.com",
      "server" : "https:\\/\\/www.paypal.com\\/cgi-bin\\/webscr"
    },
    "enabled" : true,
    "name" : "paypal_vaxia_fund",
    "title_specific" : "Paypal - Vaxia Fund",
    "title_generic" : "Paypal",
    "uid" : 1,
    "controller_class_name" : "PayPalPaymentPPSPaymentMethodController"
  }');
  return $items;
}
