#include "sensor_chip.h"

#include "common.h"

SensorChip::SensorChip(int data_available_pin)
    : accel_gyro_(0x6b),
      data_available_pin_(data_available_pin) {}

void SensorChip::Setup() {
  uint8_t who_am_i = accel_gyro_.ReadByteRegister(0xf); // WHO_AM_I
  if (who_am_i != 0b01101000)
    halt("LSM9DS1 not detected");

  // CTRL_REG1_G
  // ODR_G = 119 Hz
  // FS_G = 245 dps
  accel_gyro_.WriteRegister(0x10, 0b01100000);

  // INT1_CTRL : INT_DRDY_G
  accel_gyro_.WriteRegister(0x0C, 0b00000010);

  // FIFO_CTRL
  // accel_gyro_.WriteRegister(0x2e, 0b00100000);
  Serial.println("Sensors set up");
}

bool SensorChip::GyroDataAvailable() {
  return digitalRead(data_available_pin_) == HIGH;
}

void SensorChip::ReadAccelData(int16_t* x, int16_t* y, int16_t* z) {
  int16_t buffer[3];
  accel_gyro_.ReadRegister(0x28, sizeof(buffer), (byte*)buffer); // OUT_X_XL
  *x = buffer[0];
  *y = buffer[1];
  *z = buffer[2];
}

void SensorChip::ReadGyroData(int16_t* x, int16_t* y, int16_t* z) {
  int16_t buffer[3];
  accel_gyro_.ReadRegister(0x18, sizeof(buffer), (byte*)buffer); // OUT_X_G
  *x = buffer[0];
  *y = buffer[1];
  *z = buffer[2];
}
