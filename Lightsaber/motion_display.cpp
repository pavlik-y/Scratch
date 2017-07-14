#include "motion_display.h"

MotionDisplay::MotionDisplay(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor)
    : strip_(strip),
      sensor_(sensor),
      sequence_start_time_(0),
      current_frame_(0),
      direction_(1) {
}

void MotionDisplay::Start() {
  sequence_start_time_ = millis();
  current_frame_ = 0;
  OnSequenceStart();
}

void MotionDisplay::Stop() {
  strip_->clear();
  strip_->setBrightness(255);
  strip_->show();
}

void MotionDisplay::Tick() {
  unsigned long now = millis();
  sensor_->readGyro();
  long gyro = (long)sensor_->gyroData.z;
  if (gyro * direction_ > 5000) {
    direction_ = -direction_;
    if (direction_ == 1)
      RestartSequence(now);
  }

  int frame_to_show = (now - sequence_start_time_) * (pattern_len_ - 1) * 2 / sequence_interval_;
  if (frame_to_show != current_frame_) {
    current_frame_ = frame_to_show;
    ShowFrame(current_frame_);
  }
}

void MotionDisplay::SetPatternLen(int pattern_len) {
  pattern_len_ = pattern_len;
}

void MotionDisplay::OnSequenceStart() {}

void MotionDisplay::ShowFrame(int frame_idx) {
  if (frame_idx < pattern_len_ - 1) {
    ShowPattern(frame_idx, strip_);
    return;
  }
  if (frame_idx < (pattern_len_ - 1) * 2) {
    ShowPattern(2 * (pattern_len_ - 1) - frame_idx, strip_);
    return;
  }
  strip_->clear();
  strip_->show();
}

void MotionDisplay::RestartSequence(unsigned long now) {
  sequence_interval_ = now - sequence_start_time_;
  sequence_start_time_ = now;
  OnSequenceStart();
}

