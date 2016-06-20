#include "test_command_handler.h"

#include <Arduino.h>

TestCommandHandler::TestCommandHandler() {
}

TestCommandHandler::~TestCommandHandler() {
}

void TestCommandHandler::HandleButton(int button, bool state) {
  Serial.println(F("ButtonPressed"));
  Serial.println(button);
  Serial.println(state);
}

