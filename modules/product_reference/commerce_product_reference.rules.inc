<?php
// $Id$

/**
 * @file
 * Rules integration for the Product Reference module.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 *
 * Defines the event used to calculate the sell price of a product. This event
 * is governed by this module since it defines the product line item type used
 * as a variable and also defines the field used to display products to users
 * for purchase.
 */
function commerce_product_reference_rules_event_info() {
  $items = array();

  $items['commerce_product_calculate_sell_price'] = array(
    'label' => t('Calculating the sell price of a product'),
    'group' => t('Commerce Product'),
    'variables' => array(
      'line_item' => array(
        'label' => t('Product line item'),
        'type' => 'commerce_line_item',
        'skip save' => TRUE,
      ),
      'line_item_unchanged' => array(
        'label' => t('Unchanged product line item'),
        'type' => 'commerce_line_item',
        'skip save' => TRUE,
        'handler' => 'rules_events_entity_unchanged',
      ),
    ),
    'access callback' => 'commerce_order_rules_access',
  );

  return $items;
}

/**
 * Implements hook_rules_data_info().
 *
 * Used to allow use commerce_product_reference fields as commerce_product.
 * @see commerce_product_condition_product_is_of_type()
 */
function commerce_product_reference_rules_data_info() {
  return array(
    'commerce_product_reference' => array(
      'label' => t('Product reference'),
      'parent' => 'commerce_product',
    ),
  );
}
