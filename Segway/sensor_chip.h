#ifndef SENSOR_CHIP_H_
#define SENSOR_CHIP_H_

#include <Arduino.h>

#include "i2c_device.h"

// Abstracts device id and registers of sensors chip LSM9DS1.
// www.st.com/resource/en/datasheet/lsm9ds1.pdf
class SensorChip {
 public:
  SensorChip(int data_available_pin_);
  void Setup();
  bool GyroDataAvailable();
  void ReadAccelData(int16_t* x, int16_t* y, int16_t* z);
  void ReadGyroData(int16_t* x, int16_t* y, int16_t* z);

  // Reflects sensor scale. Multiply reading by this value to get deg/sec
  static constexpr double kGyroFactor = 8.75 / 1000.0;

 private:
  I2CDevice accel_gyro_;
  const int data_available_pin_;
};

#endif  // SENSOR_CHIP_H_