#ifndef ACCEL_H_
#define ACCEL_H_

#include <Arduino.h>
#include "common.h"
#include "component.h"
#include "i2c_device.h"

class Config;
class Gyro;
class SensorChip;

class Accel : public Component {
public:
  Accel();

  void Setup(SensorChip* sensors, Gyro* gyro);

  void Update() override;
  void ReadConfig(Config* config) override;

  int16_t y;
  int16_t z;
  double angle;
  Version version;

private:
  void ReadSample();

  static constexpr double kRadToDegFactor = 57.2957795131; // (1 / pi) * 180

  short yBias_;
  short zBias_;

  SensorChip* sensors_;
  Gyro* gyro_;
  Version gyro_version_;
};

#endif  // ACCEL_H_
