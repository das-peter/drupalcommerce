<?php

/**
 * Export Drupal Commerce Profiles to Features.
 *
 * This features implementation cares only about the fields assigned to a
 * customer type. The customer type itself has still to defined with the
 * hook_commerce_customer_profile_info().
 */

/**
 * Profile Type export
 */
function commerce_customer_features_api() {
  return array(
  'commerce_customer' => array(
    'name' => t('Commerce Customer Profile Types'),
    'default_hook' => 'customer_default_types',
    'features_source' => TRUE,
    ),
  );
}

/**
 * Defines the default hook for features integration.
 *
 * Since the commerce_customer module doesn't implement this
 * it's done here :)
 */
function commerce_customer_customer_default_types() {
  return commerce_customer_commerce_customer_profile_info();
}

/**
 * Implements hook_features_export_options().
 */
function commerce_customer_features_export_options() {
  $options = array();
  $profile_types = commerce_customer_profile_types();
  if (!empty($profile_types)) {
    foreach ($profile_types as $type) {
      $options[$type->type] = $type->name;
    }
  }
  return $options;
}

/**
* Build the export information for features.
*/
function commerce_customer_features_export($data, &$export, $module_name = '') {
  $pipe = array();

  // Add commerce_customer_profile as dependency
  $export['dependencies']['commerce_customer'] = 'commerce_customer';

  foreach ($data as $type) {
    $export['features']['commerce_customer'][$type] = $type;

    // Add module dependencies
    $info = commerce_customer_profile_type_load($type);
    $export['dependencies'][$info->module] = $info->module;

    // Fetch fields of the profile type and add them as dependency
    $fields = field_info_instances('commerce_customer_profile', $type);
    foreach ($fields as $name => $field) {
      $pipe['field'][] = "commerce_customer_profile-{$field['bundle']}-{$field['field_name']}";
    }
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function commerce_customer_features_export_render($module, $data) {
  // Return nothing, since the appropriate code has to exist already
  return array();
}


/**
 * Implements hook_features_revert().
 */
function commerce_customer_features_revert($module) {
  // Nothing to do here besides recaching - fields are handled by the fields
  // implementation of features.

  // Re-Cache
  drupal_static_reset('commerce_customer_profile_types');
  commerce_customer_profile_types();

  return TRUE;
}

/**
 * Implements hook_features_rebuild().
 */
function commerce_customer_features_rebuild($module) {
  return commerce_customer_features_revert($module);
}
