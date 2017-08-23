#include "position.h"

#include "command_buffer.h"
#include "config.h"
#include "motor.h"

//static const double kPosFactor = 0.015625;
static const double kPosFactor = 0.015625 * 2.0;

void Position::Setup(MotorEncoder* right_encoder) {
  right_encoder_ = right_encoder;
  sample_time = 0;
  version = 0;
  pos = 0;
  last_pos_ = 0;
  velocity = 0;
  avg_velocity = 0;
  lambda_ = 0;
  dump_ = false;
}

void Position::ReadConfig(Config* config) {
  lambda_ = config->ReadFloat_P(kPos_Lambda);
  sample_interval_ = config->ReadFloat_P(kPos_SampleInterval);
}

void Position::Update() {
  unsigned long now = millis();
  if (ElapsedTime(sample_time, now) < sample_interval_)
    return;
//    pos = double(left_encoder_->pos + right_encoder_->pos) * kPosFactor;
  pos = double(right_encoder_->pos) * kPosFactor;
  velocity = (pos - last_pos_) / (double (now - sample_time) * 0.001);
  avg_velocity = LowPassFilter(avg_velocity, velocity, lambda_);
  if (dump_) {
    Serial.print(pos);
    Serial.print(",");
    Serial.print(velocity);
    Serial.print(",");
    Serial.println(avg_velocity);
  }
  last_pos_ = pos;
  sample_time = now;
  ++version;
}

bool Position::HandleCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("RdPos")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(pos);
    cb.WriteValue(velocity);
    cb.WriteValue(avg_velocity);
    cb.EndResponse();
    return true;
  }
  return false;
}
