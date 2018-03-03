#include "diag.h"

#include "command_buffer.h"
#include "accel.h"
#include "gyro.h"

namespace {

static constexpr int kSampleInterval = 3000;

}  // namespace

Diag::Diag()
    : last_sample_time_(0) {}

void Diag::Setup(Gyro* gyro, Accel* accel) {
  accel_ = accel;
  gyro_ = gyro;
  gyro_version_ = gyro_->version;
}

void Diag::Update() {
  // unsigned long now = millis();
  // if (now - last_sample_time_ < kSampleInterval)
  //   return;
  // last_sample_time_ = now;
  if (!enabled_ || gyro_version_ == gyro_->version)
    return;
  gyro_version_ = gyro_->version;
  gyro_angle_ += gyro_->rate;
  Serial.printf("%f,%f,0\n", accel_->angle, gyro_angle_);
}

bool Diag::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("Diag")) == 0) {
    cb.BeginResponse();
    enabled_ = cb.GetIntParam(0) != 0;
    if (enabled_)
      gyro_angle_ = accel_->angle;
    cb.WriteValue("Ok");
    cb.EndResponse();
    return true;
  }
  return false;
}
