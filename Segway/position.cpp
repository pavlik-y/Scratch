#include "position.h"

#include <SwRotaryEncoder.h>

#include "command_buffer.h"
#include "common.h"
#include "config.h"

  // 960 ticks for one rotation of the wheel
  // D = 90mm
  // 285 mm

static const double kPosFactor = 0.0296875;

void Position::Setup(
    SwRotaryEncoder* left_encoder, SwRotaryEncoder* right_encoder) {
  left_encoder_ = left_encoder;
  right_encoder_ = right_encoder;
  sample_time_micros = micros();
  left_pos = (double)left_encoder_->readAbs() * kPosFactor;
  right_pos = (double)right_encoder_->readAbs() * kPosFactor;
  version = 0;
}

void Position::ReadConfig(Config* config) {
  sample_interval_micros_ =
      (unsigned long)config->ReadFloat_P(kPos_SampleInterval);
}

void Position::Update() {
  unsigned long now = micros();
  if (now - sample_time_micros < sample_interval_micros_)
    return;
  sample_time_micros = now;
  ++version;

  left_pos = (double)left_encoder_->readAbs() * kPosFactor;
  right_pos = (double)right_encoder_->readAbs() * kPosFactor;
}
