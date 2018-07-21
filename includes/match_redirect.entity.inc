<?php


/**
 * MatchRedirectEntity extends Entity.
 *
 * A subclass of Entity is a requirement for creating a custom entity. It is
 * also a minimum requirement to define the methods included here, even as
 * empty functions.
 */
class MatchRedirectEntity extends Entity {

  /**
   * Implements EntityInterface::id().
   */
  public function id() {
  }

  /**
   * Implements EntityInterface::entityType().
   */
  public function entityType() {
    return 'match_redirect';
  }

  /**
   * Implements EntityInterface::bundle().
   */
  public function bundle() {
  }

  /**
   * Implements EntityInterface::label().
   */
  public function label() {
  }

  /**
   * Implements EntityInterface::uri().
   */
  public function uri() {
  }
}


/**
 * MatchRedirectEntityControllerInterface definition.
 *
 * We create an interface here because anyone could come along and
 * use hook_entity_info_alter() to change our controller class.
 * We want to let them know what methods our class needs in order
 * to function with the rest of the module, so here's a handy list.
 *
 * @see hook_entity_info_alter()
 */
interface MatchRedirectEntityControllerInterface
  extends EntityControllerInterface {

  /**
   * Create an entity.
   */
  public function create($values);

  /**
   * Save an entity.
   *
   * @param object $entity
   *   The entity to save.
   */
  public function save($entity);

  /**
   * Delete an entity.
   *
   * @param object $entity
   *   The entity to delete.
   */
  public function delete($entity);

}

/**
 * MatchRedirectEntityController extends BackdropDefaultEntityController.
 *
 * Our subclass of BackdropDefaultEntityController lets us add a few
 * important create, update, and delete methods.
 */
class MatchRedirectEntityController
  extends DefaultEntityController
  implements MatchRedirectEntityControllerInterface {

  /**
   * Create an entity.
   */
  public function create($values) {
    // It is important when creating entities in Backdrop to ensure that you
    // create an instance of the core Entity class rather than an instance of
    // stdClass(). This would lead to exceptions on attempting to save your
    // custom entity.
    // See https://github.com/backdrop/backdrop-issues/issues/2558
    $entity = new MatchRedirectEntity();
    $entity->rid = NULL;
    $entity->source_pattern = $values['source_pattern'];
    $entity->target = $values['target'];
    $entity->status_code = $values['status_code'];
    $entity->override = $values['override'];
    $entity->query = $values['query'];
    $entity->weight = $values['weight'];
    return $entity;
  }

  /**
   * Save an entity.
   *
   * @param object $redirect
   *   The entity to save.
   */
  public function save($redirect) {
    $transaction = db_transaction();
    try {
      // Load the stored entity, if any.
      if (!empty($redirect->rid) && !isset($redirect->original)) {
        $redirect->original = entity_load_unchanged('match_redirect', $redirect->rid);
      }

      // Invoke hook_entity_presave() on other modules.
      module_invoke_all('entity_presave', $redirect, 'match_redirect');

      // Save to the database and fire relevent hooks.
      if (!empty($redirect->rid)) {

        backdrop_write_record('match_redirect', $redirect, array('rid'));
        // Invoke hook_entity_update() on other modules.
        module_invoke_all('entity_update', $redirect, 'match_redirect');
      }
      else {
        backdrop_write_record('match_redirect', $redirect);
        // Invoke hook_entity_insert() on other modules.
        module_invoke_all('entity_insert', $redirect, 'match_redirect');
      }
    } catch (Exception $e) {
      $transaction->rollback('match_redirect');
      watchdog_exception('match_redirect', $e);
      throw $e;
    }
  }

  /**
   * Delete an entity.
   *
   * @param object $entity
   *   The entity to delete.
   */
  public function delete($rids) {
    $transaction = db_transaction();
    try {
      foreach ($rids as $id) {
        $redirect = entity_load('match_redirect', $id);
        // Invoke hook_entity_delete() on other modules.
        module_invoke_all('entity_delete', $redirect, 'match_redirect');

        db_delete('match_redirect')
          ->condition('rid', $redirect->rid, '=')
          ->execute();
      }
    } catch (Exception $e) {
      $transaction->rollback('match_redirect');
      watchdog_exception('match_redirect', $e);
      throw $e;
    }
  }
}
