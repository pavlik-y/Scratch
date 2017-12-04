#include "common.h"

#include <Arduino.h>

void halt(const char* msg) {
  Serial.println(msg);
  halt(500);
}

void halt(int pulseTime) {
  for (int i = 0; i < 13; i++)
    digitalWrite(i, LOW);
  while(true) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(pulseTime);
    digitalWrite(LED_BUILTIN, LOW);
    delay(pulseTime);
  }
}

double ElapsedTime(unsigned long then, unsigned long now) {
  return double(now - then) * 0.001;
}

double LowPassFilter(double value, double sample, double lambda) {
  return value + lambda * (sample - value);
}
