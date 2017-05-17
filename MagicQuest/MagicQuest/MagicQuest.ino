#include <Arduino.h>

#include <Adafruit_NeoPixel.h>
#include <IRremote.h>

const byte IRpin = 2;
const byte ledPin = 13;

IRrecv ir(IRpin);
Adafruit_NeoPixel led(1);

void setup() {
  Serial.begin(115200);
  Serial.println("Analyze IR Remote");
  pinMode(ledPin, OUTPUT);
  pinMode(IRpin, INPUT);
  ir.enableIRIn();
  led.begin();
  led.clear();
  led.show();
}

void loop() {
  decode_results results;
  if (ir.decode(&results)) {
    ir.resume();
    Serial.println("Decoded");
    if (results.decode_type == MAGIQUEST) {
      Serial.println(results.value, HEX);
      for (int i = 0; i < 4; i++) {
        led.setPixelColor(0, 255, 0, 0);
        led.show();
        delay(100);
        led.setPixelColor(0, 0, 255, 0);
        led.show();
        delay(100);
        led.setPixelColor(0, 0, 0, 255);
        led.show();
        delay(100);
      }
      led.clear();
      led.show();
    }
  }
}

