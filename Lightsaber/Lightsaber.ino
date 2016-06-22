#include <Arduino.h>

#include <Adafruit_BluefruitLE_SPI.h>
#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>

#include "blinker.h"
#include "command_handler.h"
#include "command_parser.h"
#include "component_driver.h"

#define LED_PIN 6
#define BLUEFRUIT_SPI_CS 8
#define BLUEFRUIT_SPI_IRQ 7
#define BLUEFRUIT_SPI_RST 4

Adafruit_NeoPixel strip(21, LED_PIN, NEO_RGB + NEO_KHZ800);
Adafruit_LSM9DS0 sensor;
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

Blinker blinker(&strip);

ComponentDriver component_driver;
CommandHandler command_handler(&component_driver, &blinker);
CommandParser command_parser(&ble, &command_handler);


void halt(int delay_ms) {
  while(true) {
    digitalWrite(13, HIGH);
    delay(delay_ms);
    digitalWrite(13, LOW);
    delay(delay_ms);
  }
}

void setup() {
  strip.begin();
  strip.show();

  while (!Serial);  // required for Flora & Micro
  Serial.begin(9600);
  Serial.println(F("-----------------------------------------"));

//  if (!sensor.begin()) {
//    Serial.println(F("sensor init failed"));
//    halt(1000);
//  }
  
  if (!ble.begin(false)) {
    Serial.println(F("BLE init failed"));
    halt(2000);
  }
  ble.info();
  ble.setMode(BLUEFRUIT_MODE_DATA);

  command_parser.Init();
  blinker.Register(&component_driver);
}

void loop() {
  command_parser.Tick();
  component_driver.Tick();
}
