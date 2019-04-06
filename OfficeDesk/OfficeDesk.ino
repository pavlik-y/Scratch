#include <Arduino.h>
#include <Wire.h>

#include "bluefruit.h"

#include <Adafruit_SSD1306.h>

#include "button_handler.h"
#include "command_parser.h"
#include "controller.h"
#include "desk_controller.h"
#include "desk_protocol.h"

// http://www.plantuml.com/plantuml/uml/SoWkIImgAStDuSfFoafDBb5mIItEXh6TIajAylEAuToSqjII_FoI39Y0StvUIL5-Javg4KmaEBKeCn5kbh-FgmiN5qALWguTn7mG0UGW6m0tN44NLGBg27HXY4teeWWcI0Rg08VKl1IW3m40

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

BLEUart bleuart;
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

DeskController desk_controller;
Controller controller(&desk_controller);
ButtonHandler button_handler(&controller);
CommandParser command_parser(&bleuart, &controller);
DeskProtocol desk_protocol(&controller);

void setup() {
  Serial.begin(115200);
  pinMode(6, INPUT_PULLUP);
  while ( !Serial && digitalRead(6) == HIGH) delay(10);   // for nrf52840 with native usb

  Serial.println(F("Start"));

  if (!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("Failed to initialize oled");
  }
  oled.display();

  Bluefruit.begin();
  // Set max power. Accepted values are: -40, -30, -20, -16, -12, -8, -4, 0, 4
  Bluefruit.setTxPower(4);
  Bluefruit.setName("Pavel's desk");
  // Bluefruit.setDisconnectCallback(disconnect_callback);

  // Configure and start the BLE Uart service
  bleuart.begin();

  // Set up and start advertising
  startAdv();
  button_handler.Setup(9, 6, 5);
  desk_controller.Setup(PIN_A2, PIN_A3, PIN_A4, PIN_A5);
  desk_protocol.Setup(10, 11);
}

void startAdv(void)
{
  // Advertising packet
  Bluefruit.Advertising.addFlags(BLE_GAP_ADV_FLAGS_LE_ONLY_GENERAL_DISC_MODE);
  Bluefruit.Advertising.addTxPower();

  // Include the BLE UART (AKA 'NUS') 128-bit UUID
  Bluefruit.Advertising.addService(bleuart);

  // Secondary Scan Response packet (optional)
  // Since there is no room for 'Name' in Advertising packet
  Bluefruit.ScanResponse.addName();

  /* Start Advertising
   * - Enable auto advertising if disconnected
   * - Interval:  fast mode = 20 ms, slow mode = 152.5 ms
   * - Timeout for fast mode is 30 seconds
   * - Start(timeout) with timeout = 0 will advertise forever (until connected)
   *
   * For recommended advertising interval
   * https://developer.apple.com/library/content/qa/qa1931/_index.html
   */
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244);    // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30);      // number of seconds in fast mode
  Bluefruit.Advertising.start(0);                // 0 = Don't stop advertising after n seconds
}

void loop() {
  // command_parser.Tick();
  // desk_controller.Tick();
  // button_handler.Tick();
  desk_protocol.Tick();
}

// void disconnect_callback(uint8_t reason)
// {
//   (void) reason;

//   Serial.println();
//   Serial.println("Disconnected");
//   Serial.println("Bluefruit will start advertising again");
// }