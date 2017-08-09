#ifndef GYRO_H_
#define GYRO_H_

#include <Arduino.h>
#include "common.h"
#include "component.h"
#include "config.h"
#include "i2c_device.h"

//  L3G4200D: http://www.pololu.com/file/0J491/L3G4200D.pdf
class Gyro : public Component {
public:
  Gyro()
      : device_(0x69) {}

  void Setup() {
    byte whoAmI = device_.ReadByteRegister(0xf);
    if (whoAmI != B11010011)
      halt(2000);

    // Three lowest bits of control register are enabling x,y and z
    // respectively.
    device_.WriteRegister(0x20, B00001010);

    version = 0;
  }

  virtual void ReadConfig(Config* config) {
    bias_ = config->ReadFloat_P(kGyro_Bias);
  }

  virtual void Update() {
    if (!DataAvailable())
      return;
    ReadSample();
    ++version;
  }

  bool DataAvailable() {
    // Three lowest bits of status register 27 are indicating data available for
    // x,y and z respectively.
    byte status = device_.ReadByteRegister(0x27);
    return (status & B00000010) != 0;
  }

  // Reads rotation rate around y axis from gyro. Output is in deg/sec.
  void ReadSample() {
    short y;
    // output registers:
    // x: 28
    // y: 2A
    // z: 2C
    device_.ReadRegister(0x2a, 2, (byte*)&y);
    raw_rate_ = -y;
    rate = (double(raw_rate_) - bias_) * factor_;
    ++sample_count;
    raw_angle += (long)raw_rate_;
  }

  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("RdGyro")) == 0) {
      cb.BeginResponse();
      cb.WriteValue(rate);
      cb.WriteValue(raw_angle);
      cb.WriteValue(sample_count);
      cb.WriteValue(bias_);
      cb.EndResponse();
      return true;
    }
    return false;
  }
  void SetBias(double bias) {
    bias_ = bias;
  }

  Version version;
  double rate;
  long raw_angle;
  long sample_count;

private:
  I2CDevice device_;
  // Reflects sensor scale. Multiply reading by this value to get deg/sec
  static const double factor_ = 8.75 / 1000.0;
  double bias_;
  short raw_rate_;
};
#endif
