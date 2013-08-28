<?php

function match_redirect_list_form($form, &$form_state) {
  $destination = drupal_get_destination();
  $form['#tree'] = TRUE;
  // Get all redirects
  $select = db_select('match_redirect', 'm');
  $select->fields('m');
  $select->orderBy('weight', 'ASC');
  $results = $select->execute()->fetchAll();
  foreach ($results as $row) {
    $form['redirects'][$row->rid]['pattern'] = array('#markup' => htmlentities($row->source_pattern));
    $form['redirects'][$row->rid]['target'] = array('#markup' => htmlentities($row->target));
    $form['redirects'][$row->rid]['code'] = array('#markup' => $row->status_code);
    $form['redirects'][$row->rid]['weight'] = array(
      '#type' => 'weight',
      '#title' => t('Weight'),
      '#title_display' => 'invisible',
      '#default_value' => 0,
    );
    $operations = array();
    $operations['edit'] = array(
      'title' => t('Edit'),
      'href' => 'admin/config/search/match_redirect/edit/' . $row->rid,
      'query' => $destination,
    );
    $operations['delete'] = array(
      'title' => t('Delete'),
      'href' => 'admin/config/search/match_redirect/delete/' . $row->rid,
      'query' => $destination,
    );
    $form['redirects'][$row->rid]['operations'] = array(
      '#markup' => theme('links', array('links' => $operations, 'attributes' => array('class' => array('links', 'inline', 'nowrap'))))
    );
  }
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save changes'),
  );

  return $form;
}

function match_redirect_list_form_submit($form, &$form_state) {
  if (isset($form_state['values']['redirects'])) {
    foreach ($form_state['values']['redirects'] as $rid => $weight) {
      $redirect = new stdClass();
      $redirect->rid = $rid;
      $redirect->weight = $weight['weight'];
      match_redirect_save($redirect);
    }
  }
}

function theme_match_redirect_list_form($variables) {
  $form = $variables['form'];

  $rows = array();
  foreach (element_children($form['redirects']) as $id) {
    $form['redirects'][$id]['weight']['#attributes']['class'] = array('text-format-order-weight');
    $rows[] = array(
      'data' => array(
        drupal_render($form['redirects'][$id]['pattern']),
        drupal_render($form['redirects'][$id]['target']),
        drupal_render($form['redirects'][$id]['code']),
        drupal_render($form['redirects'][$id]['weight']),
        drupal_render($form['redirects'][$id]['operations']),
      ),
      'class' => array('draggable'),
    );
  }
  if (empty($rows)) {
    $rows[] = array(
      'data' => array(
          array('data' => t('There are currently no redirects.'), 'colspan' => 5)
      ),
    );
  }
  $header = array(t('Pattern'), t('Target'), t('Code'), t('Weight'), t('Operations'));
  $output = theme('table', array('header' => $header, 'rows' => $rows, 'attributes' => array('id' => 'text-format-order')));
  $output .= drupal_render_children($form);

  drupal_add_tabledrag('text-format-order', 'order', 'sibling', 'text-format-order-weight');

  return $output;
}

function match_redirect_add_form($form, &$form_state, $existing = NULL) {
  if (!isset($existing)) {
    $existing = new stdClass();
  }
  
  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $existing->rid,
  );
  $form['source_pattern'] = array(
    '#type' => 'textarea',
    '#title' => t('Pattern'),
    '#default_value' => $existing->source_pattern,
    '#required' => true,
  );
  $form['target'] = array(
    '#type' => 'textfield',
    '#title' => t('Target'),
    '#default_value' => $existing->target,
    '#required' => true,
  );
  $options = array(
    300 => t('300 (Multiple Choices)'),
    301 => t('301 (Moved Permanently)'),
    302 => t('302 (Found)'),
    303 => t('303 (See Other)'),
    304 => t('304 (Not Modified)'),
    305 => t('305 (Use Proxy)'),
    307 => t('307 (Temporary Redirect)'),
  );
  $form['status_code'] = array(
    '#type' => 'select',
    '#title' => t('Status Code'),
    '#options' => $options,
    '#default_value' => $existing->status_code,
    '#required' => true,
  );
  $form['override'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow content to be redirected?'),
    '#default_value' => $existing->override,
  );
  $form['weight'] = array(
    '#type' => 'weight',
    '#title' => t('Weight'),
    '#default_value' => isset($existing->weight) ? $existing->weight : 0,
  );
  $form['actions'] = array('#type' => 'actions');
  $form['actions']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save'),
  );
  return $form;
}

function match_redirect_add_form_submit($form, &$form_state) {
  form_state_values_clean($form_state);
  $values = (object)$form_state['values'];
  match_redirect_save($values);
  drupal_set_message(t('Redirect saved.'));
  $form_state['redirect'] = 'admin/config/search/match_redirect';
}

function match_redirect_delete_form($form, &$form_state, $redirect) {
  $form['rid'] = array(
    '#type' => 'value',
    '#value' => $redirect->rid,
  );
  return confirm_form(
    $form,
    t('Are you sure you want to delete the redirect to %target?', 
    array('%target' => $redirect->target)),
    'admin/config/search/match_redirect'
  );
}

function match_redirect_delete_form_submit($form, &$form_state) {
  match_redirect_delete($form_state['values']['rid']);
  drupal_set_message(t('Redirect has been deleted.'));
  $form_state['redirect'] = 'admin/config/search/match_redirect';
}