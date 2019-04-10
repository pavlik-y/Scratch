#include "controller.h"

#include <Arduino.h>

#include "desk_controller.h"

Controller::Controller(DeskController* desk_controller, Adafruit_SSD1306* oled)
    : desk_controller_(desk_controller),
      oled_(oled) {}

void Controller::ButtonPressed(int button) {
  Serial.printf("ButtonPressed:%x\n", button);
  desk_controller_->SetCommand(button);
}

void Controller::ButtonReleased(int button) {
  Serial.printf("ButtonReleased:%x\n", button);
  desk_controller_->ResetCommand(button);
}

void Controller::Tick() {
  uint32_t now = millis();
  switch (state_) {
    case State::IDLE:
      for (int button = 0; button < 2; button++) {
        if (desk_controller_->DoubleClickDetected(button)) {
          Serial.printf("dblclck %d\n", button);
          latched_start_time_ = millis();
          state_ = State::LATCHED;
          digitalWrite(LED_BUILTIN, HIGH);
          latched_command_ = 1<<(button+2);
          desk_controller_->SetCommand(latched_command_);
          break;
        }
      }
      break;
    case State::LATCHED:
      if ((now - latched_start_time_) >= 15000 ||
          desk_controller_->ExternalInput()) {
        Serial.println("Release");
        desk_controller_->ResetCommand(latched_command_);
        state_ = State::IDLE;
        digitalWrite(LED_BUILTIN, LOW);
      }
      break;
  }
}

#if 0

bool LogicData::IsValid(uint32_t msg) {
  if ((msg & 0xFFF00000) != 0x40600000) {
    return false;
  }
  return CheckParity(msg);
}

bool LogicData::IsNumber(uint32_t msg) {
  return IsValid(msg) && (msg & 0xFFF0000 xFFE 00) == 0x00400
}

const char * LogicData::MsgType(uint32_t msg) {
  if ((msg & 0xFFF00000) != 0x40600000) {
    return "INVAL";
  }


#endif
void Controller::WordReceived(uint32_t value) {
  if ((value & 0xFFFFFE00) != 0x40600400)
    return;
  value >>= 1;
  uint32_t number = 0;
  for (int i = 0; i < 8; i++) {
    number <<= 1;
    number |= (value >> i) & 1;
  }
  oled_->clearDisplay();
  oled_->setCursor(0, 0);
  oled_->printf("%d", number);
  oled_->display();
}