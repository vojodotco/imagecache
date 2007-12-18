<?php
// $Id$

/**
 * Imagecache Scale 
 */
function imagecache_scale_form($data) {
  $form = imagecache_resize_form($data);
  $form['upscale'] = array(
    '#type' => 'checkbox',
    '#default_value' => (isset($data['upscale'])) ? $data['upscale'] : 0,
    '#title' => t('Allow Upscaling'),
    '#description' => t('Let scale make images larger than their original size'),
  );
  return $form;
}

function theme_imagecache_scale($element) {
  $output = theme_imagecache_resize($element) .  ', upscale: '. 
  $output .= ($element['#value']['upscale']) ? t('Yes') : t('No');
  return $output;
}

function imagecache_scale_image($image, $data) {
  // Set impossibly large values if the width and height aren't set.
  $data['width'] = $data['width'] ? $data['width'] : 9999999;
  $data['height'] = $data['height'] ? $data['height'] : 9999999;
  if (!$image = imageapi_image_scale($image, $data['width'], $data['height'], $data['upscale'])) {
    watchdog('imagecache', t('imagecache_scale_image failed. image: %image, data: %data.', array('%path' => $image, '%data' => print_r($data, TRUE))), WATCHDOG_ERROR);
    return FALSE;
  }
  return $image;
}


/**
 * Imagecache Scale and Crop
 */
function imagecache_scale_and_crop_form($data) {
  return imagecache_resize_form($data);
}

function theme_imagecache_scale_and_crop($element) {
  return theme_imagecache_resize($element);
}


function imagecache_scale_and_crop_image($image, $data) {
   // Set impossibly large values if the width and height aren't set.
  $data['width'] = $data['width'] ? $data['width'] : 9999999;
  $data['height'] = $data['height'] ? $data['height'] : 9999999;
  if (!$image = imageapi_image_scale_and_crop($image, $data['width'], $data['height'])) {
    watchdog('imagecache', t('imagecache_scale_and_crop failed. image: %image, data: %data.', array('%path' => $image, '%data' => print_r($data, TRUE))), WATCHDOG_ERROR);
    return FALSE;
  }
  return $image;
}



/**
 * Imagecache Deprecated Scale.
 * This will be removed in imagecache 2.1
 */
function imagecache_deprecated_scale_form($data) {
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

function theme_imagecache_deprecated_scale($element) {
  $data = $element['#value'];
  $options = array('inside' => t('Inside dimensions'), 'outside' => t('Outside dimensions'));
  return 'width: '. $data['width'] .', height: '. $data['height'] .', fit: '. $options[$data['fit']];
}

function imagecache_deprecated_scale_image($image, $data) {
  if ($data['fit'] == 'outside' && $data['width'] && $data['height']) {
    $ratio = $image->info['width'] / $image->info['height'];
    $new_ratio = $data['width']/$data['height'];
    $data['width'] = $ratio > $new_ratio ? 0 : $data['width'];
    $data['height'] = $ratio < $new_ratio ? 0 : $data['height'];
  }
  // Set impossibly large values if the width and height aren't set.
  $data['width'] = $data['width'] ? $data['width'] : 9999999;
  $data['height'] = $data['height'] ? $data['height'] : 9999999;
  if (!$image = imageapi_image_scale($image, $data['width'], $data['height'])) {
     watchdog('imagecache', t('imagecache_deprecated_scale failed. image: %image, data: %data.', array('%path' => $image, '%data' => print_r($data, TRUE))), WATCHDOG_ERROR);
    return FALSE;
  }
  return $image;
}



/**
 * Imagecache Crop 
 */
function imagecache_crop_form($data) {
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

function theme_imagecache_crop($element) {
  $data = $element['#value'];
  return 'width: '. $data['width'] .', height: '. $data['height'] .', xoffset: '. $data['xoffset'] .', yoffset: '. $data['yoffset'];
}

function imagecache_crop_image($image, $data) {
  if (!$image = imageapi_image_crop($image, $data['xoffset'], $data['yoffset'], $data['width'], $data['height'])) {
      watchdog('imagecache', t('imagecache_crop failed. image: %image, data: %data.', array('%path' => $image, '%data' => print_r($data, TRUE))), WATCHDOG_ERROR);
    return FALSE;
  }
  return $image;
}


/**
 * Imagecache Desaturate
 */
function imagecache_desaturate_form($data) {
  return array();
}

function theme_imagecache_desaturate($element) {
  return '';
}


function imagecache_desaturate_image($image, $data = array()) {
   if (!$image = imageapi_image_desaturate($image)) {
      watchdog('imagecache', t('imagecache_desaturate failed. image: %image, data: %data.', array('%path' => $image, '%data' => print_r($data, TRUE))), WATCHDOG_ERROR);
    return FALSE;
  }
  return $image;
}

if (!function_exists('image_gd_desaturate')) {
  function image_gd_desaturate($source, $destination) {
    $info = image_get_info($source);
    if (!$info) {
      return FALSE;
    }
    $width  = $info['width'];
    $height = $info['height'];

    $im = image_gd_open($source, $info['extension']);

    if (function_exists('imagefilter')) { // PHP 5
      if (!imagefilter($im, IMG_FILTER_GRAYSCALE)) {
       return FALSE;
      }
      $result = image_gd_close($im, $destination, $info['extension']);
      imageDestroy($im);

    } else {                              // PHP < 5
      $res = imageCreateTrueColor($width, $height);
      
      for ($y = 0; $y < $height; ++$y)
       for ($x = 0; $x < $width; ++$x) {
         $rgb = imagecolorat($im, $x, $y);
         if (imageistruecolor($im)) {
           $red   = ($rgb >> 16) & 0xFF;
           $green = ($rgb >> 8)  & 0xFF;
           $blue  = $rgb & 0xFF;
         } else {
           $rgb = imagecolorsforindex($im, $rgb);
           $red   = $rgb['red'];
           $green = $rgb['green'];
           $blue  = $rgb['blue'];
         }
         
         $gray = round(.299*$red + .587*$green + .114*$blue);
         
         // shift gray level to the left
         $grayR = $gray << 16;   // R: red
         $grayG = $gray << 8;    // G: green
         $grayB = $gray;         // B: blue
         $grayColor = $grayR | $grayG | $grayB;
         
         // set the pixel color
         imagesetpixel($res, $x, $y, $grayColor);
         imagecolorallocate($res, $gray, $gray, $gray);
       }
      
      $result = image_gd_close($res, $destination, $info['extension']);
      
      imageDestroy($res);
      imageDestroy($im);
    }
    
    return $result;
  }
}

if (!function_exists('image_scale_and_crop')) {
  /**
   * Scales an image to the given width and height by scaling and cropping.
   *
   * The resulting image always has the exact target dimensions.
   *
   * @param $source         The file path of the source image
   * @param $destination    The file path of the destination image
   * @param $width          The target width
   * @param $height         The target height
   *
   * @return True or FALSE, based on success
   */
  function image_scale_and_crop($source, $destination, $width, $height) {
    $info = image_get_info($source);

    $scale = max($width / $info['width'], $height / $info['height']);
    $x = round(($info['width'] * $scale - $width) / 2);
    $y = round(($info['height'] * $scale - $height) / 2);

    if (image_toolkit_invoke('resize', array($source, $destination, $info['width'] * $scale, $info['height'] * $scale))) {
      return image_toolkit_invoke('crop', array($destination, $destination, $x, $y, $width, $height));
    }
  }
}


