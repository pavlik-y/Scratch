#include <Arduino.h>

#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
//#include <SD.h>

#include "base.h"
#include "blinker.h"
#include "command_handler.h"
#include "command_parser.h"
#include "component_driver.h"
#include "digital_clock.h"
#include "flashlight.h"
#include "prefs.h"
#include "rainbow.h"
#include "sensor_display.h"
#include "shock_flash.h"
#include "static_picture_display.h"
#include "time_bar_display.h"
// #include "time_display.h"

#define LED_PIN 6
#define BLUEFRUIT_SPI_CS 8
#define BLUEFRUIT_SPI_IRQ 7
#define BLUEFRUIT_SPI_RST 4

Adafruit_NeoPixel strip(21, LED_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_LSM9DS0 sensor;
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ,
    BLUEFRUIT_SPI_RST);
RTC_PCF8523 rtc;

//Sd2Card card;
//SdVolume volume;
//SdFile root;

Prefs prefs;

Blinker blinker(&strip);
DigitalClock digital_clock(&strip, &rtc);
Flashlight flashlight(&strip, &prefs);
Rainbow rainbow(&strip);
SensorDisplay sensor_display(&strip, &sensor);
ShockFlash shock_flash(&strip, &sensor, &prefs);
StaticPictureDisplay static_picture(&strip, &sensor);
TimeBarDisplay time_bar_display(&strip, &sensor, &rtc);
// TimeDisplay time_display(&strip, &sensor, &rtc);

ComponentDriver component_driver;
CommandHandler command_handler(&component_driver, &prefs, &rtc,
    &blinker,
    &shock_flash,
    &digital_clock,
    &sensor_display,
    &time_bar_display,
    &flashlight,
    &rainbow,
    &static_picture);
CommandParser command_parser(&ble, &command_handler);

void setup() {
  // while (!Serial);  // required for Flora & Micro
  Serial.begin(9600);
  Serial.println(F("-----------------------------------------"));

  strip.begin();
  Serial.println(F("after strip init."));
  strip.clear();
  strip.show();
  Serial.println(F("after strip show."));

  if (!sensor.begin())
    halt(F("sensor init failed"));
  Serial.println(F("after sensor init."));

  if (!rtc.begin()) {
    Serial.println(F("rtc init failed"));
    halt(3000);
  }
  Serial.println(F("after rtc init."));

  if (!ble.begin(false))
    halt(F("BLE init failed"));
  Serial.println(F("after ble init."));

  ble.setMode(BLUEFRUIT_MODE_DATA);

  static_picture.SetPredefinedPattern(0);
  // static_picture.Register(&component_driver);
  shock_flash.Register(&component_driver);
  // digital_clock.Register(&component_driver);
  // sensor_display.Register(&component_driver);
  // sensor_display.SetSensorType(SensorDisplay::GYROSCOPE);
  // flashlight.Register(&component_driver);
  // rainbow.Register(&component_driver);
  Serial.println(F("setup done."));
}

void loop() {
  command_parser.Tick();
  component_driver.Tick();
}
