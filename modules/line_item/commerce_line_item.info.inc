<?php
// $Id$

/**
 * @file
 * Provides metadata for the line item entity.
 */

/**
 * Implements hook_entity_property_info().
 */
function commerce_line_item_entity_property_info() {
  $info = array();

  // Add meta-data about the basic commerce_line_item properties.
  $properties = &$info['commerce_line_item']['properties'];

  $properties['line_item_id'] = array(
    'label' => t('Line item ID'),
    'description' => t('The internal numeric ID of the line item.'),
    'type' => 'integer',
  );
  $properties['type'] = array(
    'label' => t('Type'),
    'description' => t('The human readable name of the line item type.'),
    'type' => 'text',
    'options list' => 'commerce_line_item_type_get_name',
    'required' => TRUE,
    'query callback' => 'entity_metadata_table_query',
  );
  $properties['line_item_label'] = array(
    'label' => t('Line item label'),
    'description' => t('The label displayed with the line item.'),
    'type' => 'text',
    'setter callback' => 'entity_metadata_verbatim_set',
    'required' => TRUE,
    'query callback' => 'entity_metadata_table_query',
  );
  $properties['quantity'] = array(
    'label' => t('Quantity'),
    'description' => t('Quantity associated with this line item'),
    'type' => 'integer',
    'getter callback' => 'entity_metadata_verbatim_get',
  );

  $properties['product'] = array(
    'label' => t('Product'),
    'description' => t('The product assigned to this line item.'),
    'type' => 'commerce_product_reference',
  );
  $properties['unit_price'] = array(
    'label' => t('Unit price'),
    'description' => t('Price per unit of this line item.'),
    'type' => 'commerce_price',
    'setter callback' => 'entity_metadata_verbatim_set',
    'getter callback' => 'entity_metadata_verbatim_get',
  );
  $properties['total'] = array(
    'label' => t('Total'),
    'description' => t('Price of all units of this line item.'),
    'type' => 'commerce_price',
    'setter callback' => 'entity_metadata_verbatim_set',
    'getter callback' => 'entity_metadata_verbatim_get',
  );
  return $info;
}
