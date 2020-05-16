#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_SSD1306.h>
#include "Adafruit_VEML7700.h"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

Adafruit_BMP085 bmp;
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_VEML7700 veml;

void setup() {
  bmp.begin();
  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Failed to initialize oled");
  }
  oled.display();
  oled.setTextSize(2);
  oled.setTextColor(WHITE);

  if (!veml.begin()) {
    Serial.println("Sensor not found");
  }
  veml.setGain(VEML7700_GAIN_1);
  veml.setIntegrationTime(VEML7700_IT_800MS);

  veml.setLowThreshold(10000);
  veml.setHighThreshold(20000);
  veml.interruptEnable(true);
}

void loop() {
  Serial.print("Lux: "); Serial.println(veml.readLux());
  Serial.print("White: "); Serial.println(veml.readWhite());
  Serial.print("Raw ALS: "); Serial.println(veml.readALS());

  uint16_t irq = veml.interruptStatus();
  if (irq & VEML7700_INTERRUPT_LOW) {
    Serial.println("** Low threshold");
  }
  if (irq & VEML7700_INTERRUPT_HIGH) {
    Serial.println("** High threshold");
  }

  oled.clearDisplay();
  oled.setCursor(0, 0);
  oled.printf("%2.2f C\n", bmp.readTemperature());
  oled.printf("%4.2f mm", double(bmp.readPressure()) / 133.322);
  oled.display();
  delay(2000);
}