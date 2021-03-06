#include "static_picture_display.h"

// const PROGMEM byte rgb_pattern[] = {
//   0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x1,
//   0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x2,
//   0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x44, 0x4
// };

const PROGMEM byte colors_pattern[] = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x11, 0x11, 0x22, 0x22, 0x42, 0x44, 0x44, 0x00,
  0x44, 0x44, 0x04, 0x00, 0x00, 0x11, 0x11, 0x21, 0x22, 0x22, 0x00,
  0x22, 0x22, 0x42, 0x44, 0x44, 0x00, 0x00, 0x10, 0x11, 0x11, 0x00,
  0x11, 0x11, 0x21, 0x22, 0x22, 0x44, 0x44, 0x04, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x10, 0x11, 0x11, 0x22, 0x22, 0x42, 0x44, 0x44, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

// const PROGMEM byte slon_pattern[] = {
//   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00, 0x90, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00,

//   0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x90, 0x00, 0x00, 0x00, 0x00, 0x90, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x09, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x09, 0x00, 0x00,

//   0x00, 0x00, 0x00, 0x00, 0xa0, 0xa0, 0x00, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00, 0x0a, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0xa0, 0x00, 0x00, 0x00, 0x00, 0xa0, 0x00, 0x00, 0x00,

//   0x00, 0x00, 0x0a, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x0a, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00, 0xc0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00, 0x00,

//   0x00, 0x00, 0xc0, 0x00, 0x00, 0x00, 0x00, 0xc0, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0c, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//   0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//   0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
// };


StaticPictureDisplay::StaticPictureDisplay(
    Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor)
    : MotionDisplay(strip, sensor) {
}

void StaticPictureDisplay::SetPredefinedPattern(int index) {
  if (index == 0) {
    SetPattern(7, colors_pattern);
  }
}

void StaticPictureDisplay::SetPattern(int len, const PROGMEM byte* pattern) {
  SetPatternLen(len);
  pattern_ = pattern;
}

void StaticPictureDisplay::ShowPattern(int idx, Adafruit_NeoPixel* strip) {
  PROGMEM byte* frame_ptr = pattern_ + 11*idx;
  byte two_pixels;
  for (int pixel_idx = 0; pixel_idx < 21; pixel_idx++) {
    byte pixel;
    if (pixel_idx % 2 == 0) {
      two_pixels = pgm_read_byte(frame_ptr + pixel_idx / 2);
      pixel = two_pixels & 0xf;
    } else {
      pixel = two_pixels >> 4;
    }
    int level = pixel & 8 ? 128 : 255;
    int r = pixel & 1 ? level : 0;
    int g = pixel & 2 ? level : 0;
    int b = pixel & 4 ? level : 0;
    strip->setPixelColor(pixel_idx, r, g, b);
  }
  strip->show();
}