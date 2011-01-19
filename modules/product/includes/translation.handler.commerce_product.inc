<?php
// $Id$

/**
 * @file
 * Commerce product translation handler for the translation module.
 */


/**
 * Commerce product translation handler.
 *
 * Override the default behaviours to provide the needed commerce product
 * properties.
 *
 * This class is registered in the commerce_product_ui.module. If you want to
 * use it without the commerce_product_ui.module make sure you register it
 * on your own by using hook_translation_info().
 * @see hook_translation_info()
 * @see commerce_product_ui.module
 */
class TranslationCommerceProductHandler extends TranslationDefaultHandler {

  public function __construct($entity_type, $entity_info, $entity, $entity_id) {
    parent::__construct('commerce_product', $entity_info, $entity, $entity_id);
  }

  public function isRevision() {
    return !empty($this->entity->revision);
  }

  public function getHumanReadableId() {
    return $this->entity->title;
  }

  public function getLanguage() {
    return $this->entity->language;
  }

  public function getAccess($op) {
    return commerce_product_access($op, $this->entity);
  }

  protected function getStatus() {
    return (boolean) $this->entity->status;
  }
}
