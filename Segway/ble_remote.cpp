#include "ble_remote.h"

#include "motor_controller.h"
#include "velocity_controller.h"

const double BleRemote::kVelocityOffset = 30.0;
const double BleRemote::kTurnOffset = 30.0;

void BleRemote::Setup(Stream* ble, VelocityController* velocity_controller,
    MotorController* motor_controller) {
  ble_ = ble;
  velocity_controller_ = velocity_controller;
  motor_controller_ = motor_controller;
}

void BleRemote::Update() {
  if (last_processed_version_ != version_) {
    ResetButtonState();
    last_processed_version_ = version_;
  }
  if (!connected_)
    return;
  if (ReadCommand()) {
    buffer_[kButtonCommandLength - 1] = '\0';
    int button = buffer_[2] - '0';
    bool state = (buffer_[3] - '0') != 0;
    HandleButton(button, state);
  }
}

void BleRemote::SetConnected(bool connected) {
  connected_ = connected;
  version_++;
}

void BleRemote::ResetButtonState() {
  forward_ = false;
  back_ = false;
  left_ = false;
  right_ = false;
  UpdateControllers();
}

bool BleRemote::ReadCommand() {
  // Command lenght is 5
  while (ble_->available()) {
    char ch = ble_->read();
    if (ch == '!')
      buffer_index_ = 0;
    if (buffer_index_ < kBufferSize) {
      buffer_[buffer_index_] = ch;
      buffer_index_++;
    }
    if (VerifyButtonCommand())
      return true;
  }
}

bool BleRemote::VerifyButtonCommand() {
  if (buffer_index_ != kButtonCommandLength)
    return false;
  if (buffer_[0] != '!' || buffer_[1] != 'B')
    return false;

  uint8_t checksum = 0;

  for (uint8_t i = 0; i < buffer_index_ - 1; i++) {
    checksum += buffer_[i];
  }
  checksum = ~checksum;

  if (checksum != buffer_[buffer_index_ - 1])
    return false;
  return true;
}

void BleRemote::HandleButton(int button, bool state) {
  switch (button) {
    case 5:
      forward_ = state;
      break;
    case 6:
      back_ = state;
      break;
    case 7:
      left_ = state;
      break;
    case 8:
      right_ = state;
      break;
  }
  UpdateControllers();
}

void BleRemote::UpdateControllers() {
  double target_velocity = 0.0;
  if (forward_)
    target_velocity += kVelocityOffset;
  if (back_)
    target_velocity -= kVelocityOffset;
  velocity_controller_->SetTargetVelocity(target_velocity);
  Serial.printf("target_velocity:%f\n", target_velocity);

  double turn_offset = 0.0;
  if (left_)
    turn_offset += kTurnOffset;
  if(right_)
    turn_offset -= kTurnOffset;
  motor_controller_->SetTurnOffset(turn_offset);
  Serial.printf("turn_offset:%f\n", turn_offset);
}
