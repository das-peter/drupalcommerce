<?php
// $Id$

/**
 * @file
 * Rules integration for the Product reference module.
 *
 * @addtogroup rules
 * @{
 */

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

/**
 * @}
 */
