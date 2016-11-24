#include "motion_image.h"

MotionImage::MotionImage(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor)
    : strip_(strip),
      sensor_(sensor) {
}

void MotionImage::Stop() {
}

void MotionImage::Tick() {
}

