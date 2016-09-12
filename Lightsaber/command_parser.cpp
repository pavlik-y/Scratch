#include "command_parser.h"

#include <Arduino.h>
#include <Adafruit_BLE.h>

#include "command_handler.h"

#define CHECK_INTERVAL 1000

CommandParser::CommandParser(Adafruit_BLE* ble, CommandHandler* command_handler)
    : ble_(ble),
      command_handler_(command_handler),
      last_check_time_(millis()) {
}

void CommandParser::Tick() {
  unsigned long now = millis();
  if (now - last_check_time_ < CHECK_INTERVAL)
    return;
  last_check_time_ = now;
  if (!ble_->isConnected())
    return;
  while(ble_->available()) {
    char ch = ble_->peek();
    if (ch == '!' && !HandleBlueFruitCommand())
      break;
    if (ch == '#' && !HandleLightsaberCommand())
      break;
    if (ch == '%' && !HandleLightsaberDiagnostic())
      break;
  }
  ble_->flush();
}

bool CommandParser::HandleBlueFruitCommand() {
  char buffer[10];
  int idx = 0;
  buffer[idx++] = ble_->read();
  buffer[idx++] = ble_->read();
  int command_len = 0;
  switch (buffer[1]) {
    case 'B': // Controller button
      command_len = 5;
      break;
    case 'C': // Color picker
      command_len = 6;
      break;
    default:
      return false;
  }
  for (;idx < command_len; idx++)
    buffer[idx] = ble_->read();
  switch (buffer[1]) {
    case 'B':
      command_handler_->HandleButton(buffer[2] - '0', buffer[3] - '0');
      return true;
    case 'C':
      command_handler_->HandleColor(buffer[2], buffer[3], buffer[4]);
      return true;
  }
  return false;
}

bool CommandParser::HandleLightsaberCommand() {
  // Al commands start with '#'
  ble_->readline();
  if (strcmp_P(ble_->buffer, PSTR("#on")) == 0) {
    // code here
    return true;
  }
  return false;
}

bool CommandParser::HandleLightsaberDiagnostic() {
  return false;
}

