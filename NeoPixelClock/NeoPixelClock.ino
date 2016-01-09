#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_NeoPixel.h>

#include "clock_display.h"
#include "fade_controller.h"
#include "RTCDS1307.h"

const int PIXEL_PIN = 6;
const int DIAG_PIN = 13;
const int SQW_PIN = 2;
const int ENCODER_A_PIN = 3;
const int ENCODER_B_PIN = 4;
const int LIGHT_SENSOR_PIN = 0;

unsigned long beginning_of_second;

RTCDS1307 rtc;
FadeController fade_controller;
ClockDisplay clock_display;

void SyncBeginningOfSecond() {
  beginning_of_second = millis();
}

long encoder = 0;

void UpdateEncoder() {
  static int last_a = 0;
  int a = digitalRead(ENCODER_A_PIN);
  int b = digitalRead(ENCODER_B_PIN);
  if (a == last_a)
    return;
  last_a = a;
  int sign;
  if (a == b)
    sign = 1;
  else
    sign = -1;

  encoder += sign;
}

void setup() {
  pinMode(DIAG_PIN, OUTPUT);
  pinMode(SQW_PIN, INPUT_PULLUP);
  pinMode(ENCODER_A_PIN, INPUT_PULLUP);
  pinMode(ENCODER_B_PIN, INPUT_PULLUP);
  Serial.begin(9600);
  Wire.begin();
  rtc.Setup();
  fade_controller.Setup(PIXEL_PIN, 59);
  clock_display.Setup(&fade_controller);
  digitalWrite(DIAG_PIN, LOW);
  attachInterrupt(0, SyncBeginningOfSecond, FALLING);
  attachInterrupt(1, UpdateEncoder, CHANGE);
  if (!rtc.ReadClock()) {
    digitalWrite(13, HIGH);;
    while (true) delay(1000);
  }
  clock_display.UpdateTime(rtc.hour, rtc.min, rtc.sec, false);
  Serial.println("Restart");
}

unsigned long last_beginning_of_second = 0;
long last_encoder;

void loop() {
  if (last_beginning_of_second != beginning_of_second) {
    last_beginning_of_second = beginning_of_second;
    // Read clock
    rtc.ReadClock();
    clock_display.UpdateTime(rtc.hour, rtc.min, rtc.sec, true);

  }

  int brightness = analogRead(LIGHT_SENSOR_PIN);
  brightness = map(brightness, 0, 900, 20, 100);
  brightness = constrain(brightness, 0, 255);
  fade_controller.SetBrightness(brightness);

  if (last_encoder/2 != encoder/2) {
    int offset = (encoder - last_encoder) / 2;
    int minutes = rtc.min + rtc.hour * 60 + offset + 7200;

    rtc.SetTime((minutes / 60) % 12, minutes % 60);
    last_encoder = encoder;
    clock_display.UpdateTime(rtc.hour, rtc.min, rtc.sec, false);
  }
  fade_controller.Update();

  delay(25);
}

