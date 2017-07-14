#include <Arduino.h>

#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>
#include <RTClib.h>
//#include <SD.h>

#include "base.h"
#include "command_handler.h"
#include "command_parser.h"
#include "component_driver.h"
#include "digital_clock.h"
#include "prefs.h"
#include "sensor_display.h"
#include "shock_flash.h"
#include "static_picture_display.h"
#include "time_display.h"

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

StaticPictureDisplay static_picture(&strip, &sensor);
DigitalClock digital_clock(&strip, &rtc);
SensorDisplay sensor_display(&strip, &sensor);
ShockFlash shock_flash(&strip, &sensor, &prefs);
TimeDisplay time_display(&strip, &sensor, &rtc);

ComponentDriver component_driver;
CommandHandler command_handler(&component_driver, &prefs, &rtc,
    &static_picture,
    &shock_flash,
    &digital_clock,
    &sensor_display,
    &time_display);
CommandParser command_parser(&ble, &command_handler);

void setup() {
  strip.begin();
  strip.clear();
  strip.show();

  // while (!Serial);  // required for Flora & Micro
  Serial.begin(9600);
  Serial.println(F("-----------------------------------------"));

  if (!sensor.begin())
    halt(F("sensor init failed"));

 if (!rtc.begin()) {
   Serial.println(F("rtc init failed"));
   halt(3000);
 }

  if (!ble.begin(false))
    halt(F("BLE init failed"));

  ble.setMode(BLUEFRUIT_MODE_DATA);

//  blinker.SetPredefinedPattern(0);
//  blinker.Register(&component_driver);
//  shock_flash.Register(&component_driver);
//  digital_clock.Register(&component_driver);
//  sensor_display.Register(&component_driver);
//  sensor_display.SetSensorType(SensorDisplay::GYROSCOPE);
  time_display.Register(&component_driver);
}

void loop() {
  command_parser.Tick();
  component_driver.Tick();
}
