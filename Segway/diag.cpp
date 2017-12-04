#include "diag.h"

#include "gyro.h"

namespace {

static constexpr int kSampleInterval = 3000;

}  // namespace

Diag::Diag()
    : gyro_(nullptr),
      last_sample_time_(0) {}

void Diag::Setup(Gyro* gyro) {
  gyro_ = gyro;
}

void Diag::Update() {
  unsigned long now = millis();
  if (now - last_sample_time_ < kSampleInterval)
    return;
  last_sample_time_ = now;
  Serial.print(gyro_->raw_angle);
  Serial.print(", ");
  Serial.print(gyro_->sample_count);
  Serial.print(", ");
  Serial.println(gyro_->rate);
}