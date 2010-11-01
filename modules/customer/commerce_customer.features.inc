<?php

/**
 * Export Drupal Commerce Profiles to Features.
 */

/**
 * Profile Type export
 */
function commerce_customer_features_api() {
  return array(
  'commerce_customer' => array(
    'name' => t('Commerce Customer Profile Types'),
    'default_hook' => 'commerce_customer_commerce_customer_profile_info',
    'default_file' => FEATURES_DEFAULTS_INCLUDED,
    'features_source' => TRUE,
    )
  );
}

/**
 * Implementation of hook_features_export_options().
 */
function commerce_customer_features_export_options() {
  $options = array();
  $profile_types = commerce_customer_profile_types();
  if(!empty($profile_types)) {
    foreach($profile_types as $type) {
      $options[$type->type] = $type->name;
    }
  }
  return $options;
}

function commerce_customer_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  // Add commerce_customer_profile as dependency
  $export['dependencies']['commerce_customer'] = 'commerce_customer';

  foreach ($data as $type) {
    $export['features']['commerce_customer'][$type] = $type;

    // Add module dependencies
    $info = commerce_customer_profile_type_load($type);
    $export['dependencies'][$info->module] = $info->module;

    // Fetch fields of the profile type
    $fields = field_info_instances('commerce_customer_profile', $type);
    foreach ($fields as $name => $field) {
      $pipe['field'][] = "commerce_customer_profile-{$field['bundle']}-{$field['field_name']}";
    }
  }
  return $pipe;
}

/**
 * Implementation of hook_features_export_render().
 */
function commerce_customer_features_export_render($module = 'foo', $data) {
  $output = array();
  $output[] = '  $items = array(';
  foreach ($data as $type) {
    $info = commerce_customer_profile_type_load($type);

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
  return array('commerce_customer_profile_info' => $output);
}


/**
 * Implementation of hook_features_revert().
 */
function commerce_customer_features_revert($module) {
  // Get default profile types
  if (module_hook($module, 'commerce_customer_profile_info')) {
    // Nothing to do since the whole thing is handled via fields
    /*$default_types = module_invoke($module, 'commerce_customer_profile_info');
    $existing_types = commerce_customer_profile_types();
    foreach ($default_types as $type) {
      // Add / or update
      if (!isset($existing_types[$type->type])) {
        $type->is_new = true;
      }
      commerce_customer_profile_type_save($type);
    }*/
  }
  else {
    drupal_set_message(t('Could not load default profile types.'), 'error');
    return FALSE;
  }

  // Re-Cache
  drupal_static_reset('commerce_customer_profile_types');
  commerce_customer_profile_types();

  menu_rebuild();
  return TRUE;
}

/**
 * Implementation of hook_features_rebuild().
 */
function commerce_customer_features_rebuild($module) {
  return commerce_customer_features_revert($module);
}
