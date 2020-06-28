#include <Arduino.h>
#include <Wire.h>

#include <Adafruit_NeoPixel.h>

#include "component.h"
#include "component_manager.h"
#include "display.h"
#include "real_time_clock.h"
#include "time_display.h"

#define LED_PIN 27
#define LED_COUNT 4


Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

Display display;
RealTimeClock rtc;

constexpr size_t kMaxComponents = 16;
ComponentManager<kMaxComponents> component_manager;

TimeDisplay time_display(&rtc);

void setup() {
  Serial.begin(115200);

  while (!Serial) { delay(1); }
  Wire.begin();
  display.Setup();
  rtc.Setup();

  delay(10);
  strip.begin();
  strip.clear();
  strip.show();
  strip.show();

  component_manager.RegisterComponent(nullptr);
}

unsigned int dim_level = 0xf;
void HandleKeys() {
  display.ReadKeys();
  for (int i = 0; i < 48; i++) {
    if (display.GetKeyState(i)){
      Serial.printf("Key pressed:%d\n", i);
      display.SetDigit(0, i % 10);
      display.SetDigit(1, i / 10);
      display.SetDigit(2, 0x10);
      display.SetDigit(3, 0);
    }
  }
  if (display.GetKeyState(7)) {
    // Increase brightness;
    dim_level = (dim_level + 1) & 0xf;
    display.SetDimLevel(dim_level);
  }
  if (display.GetKeyState(6)) {
    // Decrease brightness;
    dim_level = (dim_level - 1) & 0xf;
    display.SetDimLevel(dim_level);
  }
  if (display.GetKeyState(37)) {
    rtc.minutes = (rtc.minutes + 1) % 60;
    rtc.Adjust();
  }
  if (display.GetKeyState(21)) {
    rtc.minutes = (rtc.minutes - 1) % 60;
    rtc.Adjust();
  }
}

void loop() {
  component_manager.Tick();
  rtc.Read();
  display.SetDigit(0, rtc.minutes % 10);
  display.SetDigit(1, rtc.minutes / 10);
  int hours = (rtc.hours + 11) % 12 + 1;
  display.SetDigit(2, hours % 10);
  display.SetDigit(3, hours / 10);
  display.SetStatusBit(7, rtc.hours >= 12);
  display.SetColon(true);
  HandleKeys();
  display.Show();

  delay(50);
}

void halt(int code, const char* msg) {
  display.SetDigit(0, code & 0xf);
  display.SetDigit(1, (code >> 4) & 0xf);
  display.SetDigit(2, (code >> 8) & 0xf);
  display.SetDigit(3, 0);
  strip.fill(strip.Color(255, 0, 0));
  strip.show();

  while (true) {
    Serial.println(msg);
    delay(2000);
  }
}