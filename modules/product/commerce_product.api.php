<?php

/**
 * @file
 * This file contains no working PHP code; it exists to provide additional
 * documentation for doxygen as well as to document hooks in the standard
 * Drupal manner.
 */

/**
 * Declare provided product types.
 *
 */
function hook_commerce_product_info() {
  $items = array(
    'product' => array(
      'type' => t('product'),
      'name' => t('Product'),
      'description' => t('A basic product type.'),
      'help' => '',
    ),
  );
  return $items;
}