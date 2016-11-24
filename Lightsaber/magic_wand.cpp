#include "magic_wand.h"

MagicWand::MagicWand(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor)
    : strip_(strip),
      sensor_(sensor) {
}

void MagicWand::Stop() {
}

void MagicWand::Tick() {
}

