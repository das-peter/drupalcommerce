<?php
// $Id$

/**
 * @file
 * Rules integration for line items.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_line_item_rules_event_info() {
  $items = array(
    'commerce_line_item_insert' => array(
      'label' => t('After saving a new line item'),
      'group' => t('Commerce'),
      'variables' => commerce_line_item_rules_variables(t('created line item')),
      'access callback' => 'commerce_line_item_rules_access',
    ),
    'commerce_line_item_update' => array(
      'label' => t('After updating an existing line item'),
      'group' => t('Commerce'),
      'variables' => commerce_line_item_rules_variables(t('updated line item'), TRUE),
      'access callback' => 'commerce_line_item_rules_access',
    ),
    'commerce_line_item_presave_insert' => array(
      'label' => t('Before saving a new line item'),
      'group' => t('Commerce'),
      'variables' => commerce_line_item_rules_variables(t('saved line item'), TRUE),
      'access callback' => 'commerce_line_item_rules_access',
    ),
    'commerce_line_item_presave_update' => array(
      'label' => t('Before saving an updated line item'),
      'group' => t('Commerce'),
      'variables' => commerce_line_item_rules_variables(t('saved line item'), TRUE),
      'access callback' => 'commerce_line_item_rules_access',
    ),
    'commerce_line_item_delete' => array(
      'label' => t('After deleting a line item'),
      'group' => t('Commerce'),
      'variables' => commerce_line_item_rules_variables(t('deleted line item')),
      'access callback' => 'commerce_line_item_rules_access',
    ),
  );

  // Specify that on presave the line item is saved anyway.
  $items['commerce_line_item_presave']['variables']['line_item']['skip save'] = TRUE;

  return $items;
}

/**
 * Returns some parameter suitable for using it with a node
 */
function commerce_line_item_rules_variables($label, $update = FALSE) {
  $args = array(
    'line_item' => array('type' => 'commerce_line_item', 'label' => $label),
  );

  if ($update) {
    $args += array(
      'line_item_unchanged' => array(
        'type' => 'commerce_line_item',
        'label' => t('unchanged line item'),
        'handler' => 'commerce_line_item_rules_var_unchanged',
      ),
    );
  }

  return $args;
}

/**
 * Rules integration access callback.
 */
function commerce_line_item_rules_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return TRUE;
  }
}

/**
* Implements hook_rules_action_info().
*/
function commerce_line_item_rules_action_info() {
  $actions = array();

  $actions['commerce_line_item_calculate_total'] = array(
    'label' => t('Calculate total'),
    'parameter' => array(
      'line_item' => array(
        'type' => 'commerce_line_item',
        'label' => t('Line item'),
      ),
    ),
    'group' => t('Commerce - line item'),
  );

  return $actions;
}

/**
 * Recalculate the total price of the line item.
 *
 * Usefull if unit_price is changed by a rule / hook.
 *
 * @param stdClass $line_item
 */
function commerce_line_item_calculate_total($line_item) {
  // Update the total of the line item based on the quantity and unit price.
  $line_item->total[LANGUAGE_NONE][0] = array(
    'price' => $line_item->quantity * $line_item->unit_price[LANGUAGE_NONE][0]['price'],
    'currency_code' => $line_item->unit_price[LANGUAGE_NONE][0]['currency_code'],
  );
}

/**
 * @}
 */
