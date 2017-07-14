#include "profiler.h"

#include <Arduino.h>

Profiler::Profiler()
    : elapsed_time_(0),
      sample_count_(0) {
}

void Profiler::Start() {
  start_time_ = micros();
}

void Profiler::Record() {
  elapsed_time_ += micros() - start_time_;
  sample_count_++;
  if (sample_count_ >= 193) {
    Serial.println(elapsed_time_ / sample_count_);
    elapsed_time_ = 0;
    sample_count_ = 0;
  }
}