<?php

/**
 * @file
 * Final Polish administration functions.
 */

/**
 * Form constructor for the Final Polish configuration form.
 */
function final_polish_configure($form, &$form_state) {
  $form['#submit'] = array('final_polish_configure_validate');
  $form['#submit'] = array('final_polish_configure_submit');

  $paths = variable_get('final_polish_remove_paths', array());
  $paths = implode("\n", $paths);

  $form['remove_paths'] = array(
    '#type' => 'fieldset',
    '#title' => t('Remove paths'),
    '#description' => t('If you want to remove some paths you can enter the paths here. Note that you could destroy your website!'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );

  $form['remove_paths']['final_polish_remove_paths'] = array(
    '#type' => 'textarea',
    '#title' => t('Paths to remove'),
    '#description' => t('Enter the paths you want to remove. One path per line.'),
    '#default_value' => $paths,
    '#element_validate' => array('final_polish_valid_path'),
  );

  // Allow other modules to alter this form.
  drupal_alter('final_polish_config', $form);

  return system_settings_form($form);
}

function final_polish_configure_validate($form, &$form_state) {
   $values = &$form_state['values'];

   // We need an array instead of an array.
   $paths = explode("\n", $values['final_polish_remove_paths']);

   foreach ($paths as $k => $path) {
     $paths[$k] = trim($path);
   }

   $values['final_polish_remove_paths'] = $paths;
}

/**
 * Process final_polish_configure form submissions.
 */
function final_polish_configure_submit($form, &$form_state) {
  $values = $form_state['values'];
  $paths = final_polish_get_remove_paths();

  $new_paths = $values['final_polish_remove_paths'];

  foreach ($new_paths as $k => $new_path) {
    $new_paths[$k] = trim($new_path);
  }

  if (array_diff($paths, $new_paths)) {
    menu_rebuild();
  }
}
