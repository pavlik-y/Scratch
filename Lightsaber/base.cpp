#include <Arduino.h>

#include "base.h"

void halt(const __FlashStringHelper *msg) {
  Serial.println(msg);
  while(true) {
    digitalWrite(13, HIGH);
    delay(1000);
    digitalWrite(13, LOW);
    delay(1000);
  }
}

