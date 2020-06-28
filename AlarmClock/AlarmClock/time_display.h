#ifndef TIME_DISPLAY_H_
#define TIME_DISPLAY_H_

#include "component.h"

class RealTimeClock;

class TimeDisplay : public Component {
 public:
  TimeDisplay(RealTimeClock* rtc);

  void Tick() override;

 private:
  RealTimeClock* rtc_;
};

#endif  // TIME_DISPLAY_H_