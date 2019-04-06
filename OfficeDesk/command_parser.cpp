#include "command_parser.h"

#include <Arduino.h>

#include "bluefruit.h"

#include "controller.h"

#define CHECK_INTERVAL 500

CommandParser::CommandParser(BLEUart* ble, Controller* controller)
    : ble_(ble),
      controller_(controller),
      last_char_time_(millis()) {
}

void CommandParser::Tick() {
  while(ble_->available()) {
    unsigned long now = millis();
    if (now - last_char_time_ >= CHECK_INTERVAL)
      idx = 0;
    last_char_time_ = now;
    char ch = ble_->read();
    if (ch == '!')
      idx = 0;
    if (idx < 10)
      buf[idx++] = ch;
    if (idx >= 2 && HandleBlueFruitCommand())
      idx = 0;
  }
}

bool CommandParser::HandleBlueFruitCommand() {
  if (idx != 5)
    return false;
  if (buf[1] != 'B') {  // Controller button
    return false;
  }
  int button = buf[2] - '0';
  if (button < 5 || button > 6)
    return true;
  // {PAV} Awful way to map buttons
  button -= 4;
  bool pressed = (buf[3] != '0');
  if (pressed)
    controller_->ButtonPressed(button);
  else
    controller_->ButtonReleased(button);
  return true;
}
