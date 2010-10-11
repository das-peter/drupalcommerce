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
    'default_hook' => 'commerce_product_info',
    'default_file' => FEATURES_DEFAULTS_INCLUDED,
    'features_source' => TRUE,
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
      $output[] = "  (object) array(";
      foreach ($info as $key => $t) {
        if ($t) {
          $text = str_replace("'", "\'", $info->$key);
          $text = (!empty($text) && $key !='type') ? "t('{$text}')" : "'{$text}'";
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
  // Get default product types
  if (module_hook($module, 'commerce_product_info')) {
    $default_types = module_invoke($module, 'commerce_product_info');
    foreach ($default_types as $type) {
      db_delete('commerce_product_type')
      ->condition('type', $type->type)
      ->execute();
    }
  }
  else {
    drupal_set_message(t('Could not load default product types.'), 'error');
    return FALSE;
  }

  commerce_product_types_rebuild();
  menu_rebuild();
  return TRUE;
}


/**
 * Implementation of hook_features_rebuild().
 */
function commerce_product_features_rebuild($module) {
  return commerce_product_features_revert($module);
}