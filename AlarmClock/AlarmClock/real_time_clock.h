#ifndef REAL_TIME_CLOCK_H_
#define REAL_TIME_CLOCK_H_

#include "i2c_device.h"

class RealTimeClock {
 public:
  RealTimeClock();

  void Setup();

  void Read();
  void Adjust();

  int seconds;
  int minutes;
  int hours;

 private:
  I2CDevice rtc_;
};

#endif  // REAL_TIME_CLOCK_H_