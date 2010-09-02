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
  $type_map = commerce_product_types();
  $module_map = array();
  
  foreach ($type_map as $module) {
    
    $callback = "commerce_product_entity_info";
    $module_map[$module_type] = $module;
 
    $node_types = call_user_func($callback);
    if (is_array($node_types)) {
      foreach ($node_types as $module_type => $module_info) {
        $module_map[$module_type] = $module;
      }
    }
  }

  foreach ($data as $type) {
    $export['dependencies']['commerce_product'] = 'commerce_product';
    $export['dependencies']['features'] = 'features';
    $export['features']['commerce_product'][$type] = $type;
    
    $fields = field_info_instances('commerce_product', $type);
    foreach ($fields as $name => $field) {
      $pipe['field'][] = "{$field['bundle']}-{$field['field_name']}";
    }
  }
  return $pipe;
}

/**
 * Implementation of hook_features_export_render().
 */
function commerce_product_features_export_render($module = 'foo', $data) {
  $translatables = $code = array();

  $code[] = '  $fields = array();';
  $code[] = '';
  foreach ($data as $instance) {
    list($bundle, $field_name) = explode('-', $instance);

    $field = field_info_field($field_name);
    if(!$field){
      //Outch - something is wrong :|
      //Jump to next field to keep the code clean
      continue;
    }
    
    unset($field['columns']);
    unset($field['locked']);
    unset($field['storage']);
    
    //Attache instance information
    if(isset($field['bundles'])){
      foreach($field['bundles'] as $entity_type=>$bundles){
        if(in_array($bundle, $bundles)){
            $fieldInstance = field_info_instance($entity_type, $field_name, $bundle);
            $field['instances'][$field_name][$bundle] = $fieldInstance;
            // Add any labels to translatables array.
            if (!empty($fieldInstance['widget']['label'])) {
              $translatables[] = $field['widget']['label'];
            }
            break;
        }
      }
    }

    $code[] = '  // Exported field: ' . $field_name;
    $code[] = '  $fields[] = ' . features_var_export($field, '  ') . ';';
    $code[] = '';

    // Add any labels to translatables array.
    if (!empty($field['widget']['label'])) {
      $translatables[] = $field['widget']['label'];
    }
  }
  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }
  $code[] = '  return $fields;';
  $code = implode("\n", $code);
  return array('field_default_fields' => $code, );
}


/**
 * Implementation of hook_features_revert().
 */
function commerce_product_features_revert($module) {
  field_features_rebuild($module);
}

/**
 * Implementation of hook_features_rebuild().
 * Rebuilds CCK field definitions from code defaults.
 */
function commerce_product_features_rebuild($module) {
  $fields = module_invoke($module, 'field_default_fields');
  if (!empty($fields)) {
    field_info_cache_clear();

    foreach ($fields as $field) {
      $existing_field = field_info_field($field['field_name']);
      unset($field['id']);
      if (!empty($existing_field)) {
        field_update_field($field);
      }
      else {
        field_create_field($field);
      }
   
      // update/create field instances
      if(!empty($field['bundles'])) {
        foreach($field['bundles'] as $entity_type => $bundles) {
          if(is_array($bundles)) {
            foreach($bundles as $bundle) {
              $instance = field_info_instance($entity_type, $field['field_name'], $bundle);
              if ($instance) {
                field_update_instance($instance);
              } else {
                //@TODO Is this legacy check within a dev release really necessary?
                if(isset($field['instances'])){
                  //Check if this bundle has an instance - if not go to the next one
                  if(!isset($field['instances'][$field['field_name']][$bundle])){
                    continue;
                  }
                  $fieldInstance = $field['instances'][$field['field_name']][$bundle];
                }
                else{
                  $fieldInstance = $field;
                }
                $fieldInstance['entity_type'] = $entity_type;
                $fieldInstance['bundle'] = $bundle;
                field_create_instance($fieldInstance);
              }            
            }
          }
           
        }
      }

      variable_set('menu_rebuild_needed', TRUE);
    }
  }
}

