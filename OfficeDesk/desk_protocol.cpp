#include "desk_protocol.h"

#include "controller.h"

const uint32_t kMarkInterval = 30;
const uint32_t kWaitInterval = 30000;
const uint32_t kBitInterval = 980;
const uint32_t kSampleInterval = 20;


/*
0100 0000 0110 0001 0001 0100 0000 0000 0
D: 40611400 : Display on
0100 0000 0110 0000 0001 1101 1111 1111 0
!: 40601dff
0100 0000 0110 1000 0001 0100 0000 0000 0
40681400
0100 0000 0110 0000 0000 0101 1011 0010 0
N: 406005b2 : Number
0100 0000 0110 0100 1001 1110 1111 1101 1
@: 40649efd

0100 0000 0110 0000 0001 1101 1111 1111 0
!: 40601dff
0100 0000 0110 0000 0000 0110 1011 0010 0
406006b2
0100 0000 0110 0100 1001 1110 1111 1101 1
@: 40649efd

0100 0000 0110 0000 0001 1101 1111 1110 1
!: 40601dfe
0100 0000 0110 0000 0000 0101 0110 0100 0
N: 40600564 : Number?
0100 0000 0110 0100 1001 1101 1111 1001 1
@: 40649df9
0100 0000 0110 1110 0001 0100 0000 0000 1
D: 406e1400 : Display off
*/
DeskProtocol::DeskProtocol(Controller* controller)
    : controller_(controller) {}

void DeskProtocol::Setup(int pin, int pin_out) {
  pin_ = pin;
  pin_out_ = pin_out;
  pinMode(pin_, INPUT);
  pinMode(pin_out_, OUTPUT);
}

void DeskProtocol::SetState(State state) {
  // Serial.printf("%d, %d\n", state, micros());
  if (state_ == State::IDLE)
    digitalWrite(pin_out_, HIGH);
  state_ = state;
  if (state_ == State::IDLE)
    digitalWrite(pin_out_, LOW);
}

int DeskProtocol::ReadPin() {
  return digitalRead(pin_);
}

bool DeskProtocol::ReadWord(uint32_t* value) {
  for (int i = 0; i < kWaitInterval/kSampleInterval; i++) {
    if (ReadPin() == HIGH)
      break;
    delayMicroseconds(kSampleInterval);
  }
  if (ReadPin() == LOW)
    return false;
  delayMicroseconds(kBitInterval/2);
  *value = 0;
  for (int bit_index = 0; bit_index < 32; bit_index++) {
    *value <<= 1;
    if (ReadPin() == LOW)
      *value |= 1;
    delayMicroseconds(kBitInterval);
  }
  return true;
}

void DeskProtocol::Tick() {
  int level = ReadPin();
  uint32_t now = millis();
  switch (state_) {
    case State::IDLE:
      if (level == LOW) {
        ready_time_ = now;
        SetState(State::READY);
      }
      break;
    case State::READY:
      if (now - ready_time_ >= kMarkInterval) {
        vTaskSuspendAll();
        uint32_t value = 0;
        bool success = ReadWord(&value);
        xTaskResumeAll();
        if (success) {
          Serial.printf("%WD:%x\n", value);
          controller_->WordReceived(value);
          SetState(State::IDLE);
        } else {
          // Serial.println("Failed to read");
          SetState(State::LATCHED);
        }
      } else if (level == HIGH) {
        SetState(State::IDLE);
      }
      break;
    case State::LATCHED:
      if (level == HIGH) {
        SetState(State::IDLE);
      }
      break;
  }
}