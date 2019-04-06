#include "desk_protocol.h"

#include "controller.h"

const uint32_t kMarkInterval = 40000;
const uint32_t kSampleInterval = 32000;
const uint32_t kBitInterval = 1000;

DeskProtocol* DeskProtocol::s_current_reader_ = nullptr;
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

void DeskProtocol::InterruptHandler() {
  if (s_current_reader_) {
    uint32_t now = tick2us(xTaskGetTickCountFromISR());
    s_current_reader_->HandleEdge(now);
  }
}

void DeskProtocol::RecordSample(uint32_t now) {
  if (sample_size_ < kSampleCapacity) {
    samples_[sample_size_++] = now;
  }
}

void DeskProtocol::HandleEdge(uint32_t now) {
  int level = digitalRead(pin_);
  switch (state_) {
    case State::LISTENING:
      if (level == HIGH) {
        RecordSample(now);
        SetState(State::SPACE);
      }
      break;
    case State::SPACE:
      if (level == LOW) {
        RecordSample(now);
        SetState(State::MARK);
      }
      break;
    case State::MARK:
      if (level == HIGH) {
        RecordSample(now);
        SetState(State::SPACE);
      }
      break;
  }
}

uint32_t DeskProtocol::DecodeWord() {
  uint32_t value = 0;
  int sample_index = 0;
  bool bit_value = true;
  for (int bit_index = 0; bit_index < 32; bit_index++) {
    uint32_t sample_time = kBitInterval / 2 + bit_index * kBitInterval;
    while (sample_index < sample_size_ &&
        samples_[sample_index] - samples_[0] < sample_time) {
      bit_value = !bit_value;
      sample_index++;
    }
    value <<= 1;
    if (bit_value)
      value |= 1;
  }
  return value;
}

void DeskProtocol::Tick() {
  int level = digitalRead(pin_);
  uint32_t now = micros();
  switch (state_) {
    case State::IDLE:
      if (level == LOW) {
        ready_time_ = now;
        SetState(State::READY);
      }
      break;
    case State::READY:
      if (now - ready_time_ >= kMarkInterval) {
        SetState(State::LISTENING);
        sample_size_ = 0;
        s_current_reader_ = this;
        // vTaskSuspendAll();
        attachInterrupt(pin_, InterruptHandler, CHANGE);
      } else if (level == HIGH) {
        SetState(State::IDLE);
      }
      break;
    case State::MARK:
    case State::SPACE:
      if (sample_size_ > 0 && now - samples_[0] >= kSampleInterval) {
        detachInterrupt(pin_);
        // xTaskResumeAll();
        s_current_reader_ = nullptr;
        // Collect sample
        for (int i = 0; i < sample_size_; i++)
          Serial.printf("TS:%d\n", samples_[i]);
        uint32_t value = DecodeWord();
        Serial.printf("Word: %x\n", value);
        SetState(State::IDLE);
      }
      break;
  }
}