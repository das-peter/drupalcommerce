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
  $items = array(
    'commerce_product_insert' => array(
      'label' => t('After saving a new product'),
      'group' => t('Commerce'),
      'variables' => commerce_product_rules_variables(t('created product')),
      'access callback' => 'commerce_product_rules_access',
    ),
    'commerce_product_update' => array(
      'label' => t('After updating an existing product'),
      'group' => t('Commerce'),
      'variables' => commerce_product_rules_variables(t('updated product'), TRUE),
      'access callback' => 'commerce_product_rules_access',
    ),
    'commerce_product_presave' => array(
      'label' => t('Before saving a product'),
      'group' => t('Commerce'),
      'variables' => commerce_product_rules_variables(t('saved product'), TRUE),
      'access callback' => 'commerce_product_rules_access',
    ),
    'commerce_product_delete' => array(
      'label' => t('After deleting a product'),
      'group' => t('Commerce'),
      'variables' => commerce_product_rules_variables(t('deleted product')),
      'access callback' => 'commerce_product_rules_access',
    ),
  );
  // Specify that on presave the product is saved anyway.
  $items['commerce_product_presave']['variables']['product']['skip save'] = TRUE;
  return $items;
}

/**
 * Implements hook_rules_condition_info().
 */
function commerce_product_rules_condition_info() {
  $conditions = array(
    'commerce_product_condition_product_is_of_type' => array(
      'label' => t('Product is of type'),
      'help' => t('Evaluates to TRUE if the given product is of one of the selected product types.'),
      'base' => 'commerce_product_condition_product_is_of_type',
      'parameter' => array(
        'product' => array('type' => array('commerce_product', 'commerce_product_reference'), 'label' => t('Product')),
        'product_types' => array(
          'label' => t('Product types'),
          'type' => 'list<text>',
          'options list' => 'commerce_product_rules_condition_parameter_data',
          'restriction' => 'input',
        ),
      ),
      'group' => t('Commerce'),
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
 * Provides the content type of a node as asserted metadata.
 */
function commerce_product_condition_product_is_of_type($product, $types) {
  // Handle commerce_product_reference data
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
 * Returns some parameter suitable for using it with a node
 */
function commerce_product_rules_variables($label, $update = FALSE) {
  $args = array(
    'product' => array('type' => 'commerce_product', 'label' => $label),
  );
  if ($update) {
    $args += array(
      'product_unchanged' => array(
        'type' => 'commerce_product',
        'label' => t('unchanged product'),
        'handler' => 'commerce_product_rules_var_unchanged',
      ),
    );
  }
  return $args;
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
 * @}
 */
