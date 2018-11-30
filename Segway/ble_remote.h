#ifndef BLE_REMOTE_H_
#define BLE_REMOTE_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class MotorController;
class VelocityController;

class BleRemote : public Component {
 public:
  void Setup(Stream* ble, VelocityController* velocity_controller,
      MotorController* motor_controller);

  void Update() override;

  void SetConnected(bool connected);

 private:
  void ResetButtonState();
  bool ReadCommand();
  bool VerifyButtonCommand();
  void HandleButton(int button, bool state);
  void UpdateControllers();

  static const double kVelocityOffset;
  static const double kTurnOffset;

  Stream* ble_;

  VelocityController* velocity_controller_;
  MotorController* motor_controller_;
  Version version_ = 0;
  Version last_processed_version_ = 0;
  bool connected_ = false;

  static const int kButtonCommandLength = 5;
  static const int kBufferSize = 16;
  int buffer_index_ = 0;
  uint8_t buffer_[kBufferSize];

  bool forward_ = false;
  bool back_ = false;
  bool left_ = false;
  bool right_ = false;
};

#endif  // BLE_REMOTE_H_