#include <Adafruit_NeoPixel.h>

#define LED_PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(21, LED_PIN, NEO_RGB + NEO_KHZ800);


void setup() {
  strip.begin();
  strip.show();
}

void loop() {
  // put your main code here, to run repeatedly:

}
