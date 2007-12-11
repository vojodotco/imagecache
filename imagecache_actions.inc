<?php
// $Id$

function imagecache_scale_image($image, $data) {
  $size = getimagesize($image);
  if ($data['fit'] == 'outside' && $data['width'] && $data['height']) {
    $ratio = $size[0]/$size[1];
    $new_ratio = $data['width']/$data['height'];
    $data['width'] = $ratio > $new_ratio ? 0 : $data['width'];
    $data['height'] = $ratio < $new_ratio ? 0 : $data['height'];
  }
  // Set impossibly large values if the width and height aren't set.
  $data['width'] = $data['width'] ? $data['width'] : 9999999;
  $data['height'] = $data['height'] ? $data['height'] : 9999999;
  if (!image_scale($image, $image, $data['width'], $data['height'])) {
    watchdog('imagecache', t('Imagecache scale action ID %id failed.', array('%id' => $action['actionid'])), WATCHDOG_ERROR);
  }
}

function imagecache_scale_form($data) {
  $helptext = array();
  $helptext['inside'] = t('<strong>Inside dimensions</strong>: Final dimensions will be less than or equal to the entered width and height. Useful for ensuring a maximum height and/or width.');
  $helptext['outside'] = t('<strong>Outside dimensions</strong>: Final dimensions will be greater than or equal to the entered width and height. Ideal for cropping the result to a square.');
  $description = '<ul><li>'. implode('</li><li>', $helptext) .'</li><ul>';

  $form['fit'] = array(
    '#type' => 'select',
    '#title' => t('Scale to fit'),
    '#options' => array('inside' => t('Inside dimensions'), 'outside' => t('Outside dimensions')),
    '#default_value' => $data['fit'],
    '#weight' => 1,
    '#description' => $description,
  );
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $data['width'],
    '#description' => t('Enter a width in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $data['height'],
    '#description' => t('Enter a height in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  return $form;
}

function imagecache_crop_image($image, $data) {
  if (!image_crop($image, $image, $data['xoffset'], $data['yoffset'], $data['width'], $data['height'])) {
    watchdog('imagecache', t('Imagecache crop action ID %id failed.', array('%id' => $action['actionid'])), WATCHDOG_ERROR);
  }
}

function imagecache_crop_form($action) {
  $form['width'] = array(
    '#type' => 'textfield',
    '#title' => t('Width'),
    '#default_value' => $data['width'],
    '#description' => t('Enter a width in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['height'] = array(
    '#type' => 'textfield',
    '#title' => t('Height'),
    '#default_value' => $data['height'],
    '#description' => t('Enter a height in pixels or as a percentage. i.e. 500 or 80%.'),
  );
  $form['xoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('X offset'),
    '#default_value' => $data['xoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>left</em>, <em>center</em>, or <em>right</em>.'),
  );
  $form['yoffset'] = array(
    '#type' => 'textfield',
    '#title' => t('Y offset'),
    '#default_value' => $data['yoffset'],
    '#description' => t('Enter an offset in pixels or use a keyword: <em>top</em>, <em>center</em>, or <em>bottom</em>.'),
  );
  return $form;
}

function imagecache_rotate_form($data) {
  $form['degrees'] = array(
    '#type' => 'textfield',
    '#title' => t('Degrees'),
    '#default_value' => $data['degrees'],
    '#description' => t('The number of degrees, clockwise, to rotate the image.'),
  );
  $form['random'] = array(
    '#type' => 'checkbox',
    '#title' => t('Randomize rotation'),
    '#return_value' => 1,
    '#default_value' => $data['random'],
    '#description' => t('When checked, the degrees entered above will be used as a maximum, rather than an exact amount to rotate.'),
  );
  $form['bgcolor'] = array(
    '#type' => 'textfield',
    '#title' => t('Background color'),
    '#default_value' => $data['bgcolor'],
    '#description' => t('The background color to fill behind the rotated image.'),
  );
  return $form;
}

