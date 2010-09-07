<?php

/**
 * Export Drupal Commerce products to Features.
 */

/**
 * Product Type export
 */
function commerce_product_features_api() {
  return array(
  'commerce_product' => array(
    'name' => t('Commerce Products'),
    'default_hook' => 'field_default_fields',
    'default_file' => FEATURES_DEFAULTS_INCLUDED,
    'features_source' => FALSE,
    'file' => drupal_get_path('module', 'commerce_product') .'/commerce_product.features_api.inc',
    )
  );
}

/**
 * Implementation of hook_features_export_options().
 */
function commerce_product_features_export_options() {
  $options = array();
  $product_types = commerce_product_types();
  if(!empty($product_types)) {
    foreach($product_types as $type) {
      $options[$type->type] = $type->name;
    }
  }
  return $options;
}

function commerce_product_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  // Add commerce_product as dependency
  $export['dependencies']['commerce_product'] = 'commerce_product';

  foreach ($data as $type) {
    $export['features']['commerce_product'][$type] = $type;

    $fields = field_info_instances('commerce_product', $type);
    foreach ($fields as $name => $field) {
      $pipe['field'][] = "commerce_product-{$field['bundle']}-{$field['field_name']}";
    }
  }
  return $pipe;
}

/**
 * Implementation of hook_features_export_render().
 */
function commerce_product_features_export_render($module = 'foo', $data) {
  $output = array();
  $output[] = '  $items = array(';
  foreach ($data as $type) {
    $info = commerce_product_type_load($type);
    if(!empty($info)) {
      $output[] = "    '{$type}' => array(";
      foreach ($info as $key => $t) {
        if ($t) {
          $text = str_replace("'", "\'", $info->$key);
          $text = !empty($text) ? "t('{$text}')" : "''";
          $output[] = "      '{$key}' => {$text},";
        }
        else {
          $output[] = "      '{$key}' => '{$info->$key}',";
        }
      }
      $output[] = "    ),";
    }
  }
  $output[] = '  );';
  $output[] = '  return $items;';
  $output = implode("\n", $output);
  return array('commerce_product_info' => $output);
}


/**
 * Implementation of hook_features_revert().
 */
function commerce_product_features_revert($module) {
  // Get product types
  if (module_hook($module, 'commerce_product_info')) {
    $product_types = module_invoke($module, 'commerce_product_info');
    foreach($product_types as $type_name => $type_data) {
      $product_type = commerce_product_type_new();
  
      $product_type->type = $type_name;
      $product_type->name = $type_data['name'];
      $product_type->description = $type_data['description'];
      
      commerce_product_type_get_name($type_name) === FALSE ? $is_new = TRUE : $is_new = FALSE;
      $product_type->is_new = $is_new;
    
      commerce_product_type_save($product_type);
    }
  }   
}