#ifndef ACCEL_H_
#define ACCEL_H_

#include <Arduino.h>
#include <math.h>
#include "command_buffer.h"
#include "component.h"
#include "common.h"
#include "config.h"
#include "I2CDevice.h"
#include "gyro.h"

// ADXL345: http://www.analog.com/static/imported-files/data_sheets/ADXL345.pdf
class Accel : public Component {
public:
  void Setup(Gyro* gyro) {
    device_.SetAddr(0x53);
    byte devId = device_.ReadByteRegister(0x0);
    if (devId != B11100101)
      halt(1000);
    device_.WriteRegister(0x2D, B00001000);
    device_.WriteRegister(0x31, B00001011);

    xBias_ = 0;
    zBias_ = 0;
    gyro_ = gyro;
    gyro_version_ = gyro_->version;
    version = 0;
  }
  
  virtual void Update() {
    if (gyro_version_ != gyro_->version) {
      gyro_version_ = gyro_->version;
      ReadSample();
      ++version;
    }
  }
  
  virtual void ReadConfig(Config* config) {
    xBias_ = (int)config->ReadFloat_P(kAccel_BiasX);
    zBias_ = (int)config->ReadFloat_P(kAccel_BiasZ);
  }

  void ReadSample() {
    // Registers:
    //   32: x
    //   34: y
    //   36: z
    device_.ReadRegister(0x32, 2, (byte*)&x);
    device_.ReadRegister(0x36, 2, (byte*)&z);
    x -= xBias_;
    z -= zBias_;
    angle = atan2(double(x), double(z)) * factor_;
  }

  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("RdAcc")) == 0) {
      cb.BeginResponse();
      cb.WriteValue(x);
      cb.WriteValue(z);
      cb.WriteValue(angle);
      cb.WriteValue(xBias_);
      cb.WriteValue(zBias_);
      cb.EndResponse();
      return true;
    }
    return false;
  }

  short x;
  short z;
  double angle;
  Version version;

private:
  I2CDevice device_;
  short xBias_;
  short zBias_;
  static const double factor_ = 57.2957795131; // (1 / pi) * 180
  Gyro* gyro_;
  Version gyro_version_;
};
#endif
