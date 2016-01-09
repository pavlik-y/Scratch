#ifndef clock_display_h_
#define clock_display_h_

#include <Arduino.h>

#include "fade_controller.h"

class ClockDisplay {
public:
  void Setup(FadeController* fade_controller) {
    fade_controller_ = fade_controller;
    hour_pixel_ = 0;
    minute_pixel_ = 0;
    second_pixel_ = 0;
  }
  
  void UpdateTime(int hours, int minutes, int seconds, bool transition) {
    int hour_pixel = (hours % 12) * 5 + (minutes / 12);
    if (hour_pixel_ != hour_pixel) {
      fade_controller_->StartTransition(0, transition ? hour_pixel_ : hour_pixel, hour_pixel);
      hour_pixel_ = hour_pixel;
    }
    if (minute_pixel_ != minutes) {
      fade_controller_->StartTransition(1, transition ? minute_pixel_ : minutes, minutes);
      minute_pixel_ = minutes;
    }
    if (second_pixel_ != seconds) {
      fade_controller_->StartTransition(2, transition ? second_pixel_ : seconds, seconds);
      second_pixel_ = seconds;
    }
  }
  
private:
  FadeController* fade_controller_;
  int hour_pixel_;
  int minute_pixel_;
  int second_pixel_;
};
#endif
