<?php
// $Id$

/**
 * @file
 * Rules integration for products.
 *
 * @addtogroup rules
 * @{
 */

/**
 * Implements hook_rules_event_info().
 */
function commerce_product_rules_event_info() {
  $events = array();

  $events['commerce_product_presave'] = array(
    'label' => t('Before saving a product'),
    'group' => t('Commerce Product'),
    'variables' => commerce_product_rules_event_variables(t('Product'), TRUE, TRUE),
    'access callback' => 'commerce_product_rules_access',
  );
  $events['commerce_product_insert'] = array(
    'label' => t('After saving a new product'),
    'group' => t('Commerce Product'),
    'variables' => commerce_product_rules_event_variables(t('Created product'), TRUE),
    'access callback' => 'commerce_product_rules_access',
  );
  $events['commerce_product_update'] = array(
    'label' => t('After updating an existing product'),
    'group' => t('Commerce Product'),
    'variables' => commerce_product_rules_event_variables(t('Updated product'), TRUE),
    'access callback' => 'commerce_product_rules_access',
  );
  $events['commerce_product_delete'] = array(
    'label' => t('After deleting a product'),
    'group' => t('Commerce Product'),
    'variables' => commerce_product_rules_event_variables(t('Deleted product')),
    'access callback' => 'commerce_product_rules_access',
  );

  return $events;
}

/**
 * Returns a variables array for product events.
 *
 * @param $label
 *   The label for the primary product variable.
 * @param $unchanged
 *   Boolean indicating whether or not to include the unchanged product.
 * @param $skip_save
 *   Boolean indicating whether or not the primary product variable should skip
 *     saving after event execution.
 */
function commerce_product_rules_event_variables($label, $unchanged = FALSE, $skip_save = FALSE) {
  $variables = array(
    'product' => array(
      'type' => 'commerce_product',
      'label' => $label,
      'skip save' => $skip_save,
    ),
  );

  if ($unchanged) {
    $variables['product_unchanged'] = array(
      'type' => 'commerce_product',
      'label' => t('Unchanged product'),
      'skip save' => TRUE,
      'handler' => 'rules_events_entity_unchanged',
    );
  }

  return $variables;
}

/**
 * Rules integration access callback.
 */
function commerce_product_rules_access($type, $name) {
  if ($type == 'event' || $type == 'condition') {
    return commerce_product_access('view');
  }
}

/**
 * Implements hook_rules_condition_info().
 * Defines a condition to check the product type.
 */
function commerce_product_rules_condition_info() {
  $conditions = array(
    'commerce_product_condition_product_is_of_type' => array(
      'label' => t('Product is of type'),
      'help' => t('Evaluates to TRUE if the given product is of one of the selected product types.'),
      'base' => 'commerce_product_condition_product_is_of_type',
      'parameter' => array(
        'product' => array('type' => array('commerce_product'), 'label' => t('Product')),
        'product_types' => array(
          'label' => t('Product types'),
          'type' => 'list<text>',
          'options list' => 'commerce_product_rules_condition_parameter_data',
          'restriction' => 'input',
        ),
      ),
      'group' => t('Commerce Product'),
    ),
  );
  return $conditions;
}

/**
 * Provides data for the conditions
 * @param RulesCondition $element
 * @param string $name
 *
 * @TODO Figure out how to really implement this.
 */
function commerce_product_rules_condition_parameter_data($element, $name) {
  if ($name == 'product_types') {
    return commerce_product_type_get_name();
  }
}
/**
 * Check if the give product matches one of the given types.
 *
 * @param commerce_product|array
 *   Handles product objects or arrays provided by commerce_product_refrence.
 * @return boolean
 */
function commerce_product_condition_product_is_of_type($product, $types) {
  // Handle data from commerce_product_reference.
  if (is_array($product)) {
    if (count($product)) {
      foreach ( $product as $language => $deltas) {
        foreach ( $deltas as $delta => $data) {
          if (!($product_node = commerce_product_load($data['product_id'])) || !isset($types[$product_node->type])) {
            return FALSE;
          }
        }
      }
      return TRUE;
    }
    return FALSE;
  }
  return isset($types[$product->type]);
}

/**
 * @}
 */
