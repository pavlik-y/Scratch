#include "position.h"

#include <SwRotaryEncoder.h>

#include "command_buffer.h"
#include "common.h"
#include "config.h"

//static const double kPosFactor = 0.015625;
static const double kPosFactor = 0.015625;

void Position::Setup(SwRotaryEncoder* right_encoder) {
  right_encoder_ = right_encoder;
  last_sample_time_micros = micros();
  version = 0;
  pos = 0;
  // last_pos_ = 0;
  // velocity = 0;
  // avg_velocity = 0;
  // lambda_ = 0;
  last_sample_time_micros = micros();
}

void Position::ReadConfig(Config* config) {
  // lambda_ = config->ReadFloat_P(kPos_Lambda);
  sample_interval_micros_ =
      (unsigned long)config->ReadFloat_P(kPos_SampleInterval);
}

void Position::Update() {
  unsigned long now = micros();
  if (now - last_sample_time_micros < sample_interval_micros_)
    return;
  encoder_pos = right_encoder_->readAbs();
  pos = double(encoder_pos) * kPosFactor;

  last_sample_time_micros = now;
  ++version;

  // velocity = (pos - last_pos_) / (double (now - sample_time) * 0.001);
  // avg_velocity = LowPassFilter(avg_velocity, velocity, lambda_);
  // last_pos_ = pos;
}

bool Position::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("RdPos")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(pos);
    // cb.WriteValue(velocity);
    // cb.WriteValue(avg_velocity);
    cb.EndResponse();
    return true;
  }
  return false;
}
