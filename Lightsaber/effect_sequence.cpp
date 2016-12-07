#include <Arduino.h>

#include "base.h"
#include "effect_sequence.h"

EffectSequence::EffectSequence(const Step* steps, size_t steps_count) 
    : steps_(steps),
      steps_count_(steps_count),
      start_time_(0),
      last_update_time_(0) {}

void EffectSequence::Start(unsigned long now) {
  start_time_ = now;
}

bool EffectSequence::Running() const {
  // {PAV} 0 as special value can skip. Should use separate variable for is_running_.
  return start_time_ != 0;
}

bool EffectSequence::DisplayUpdateNeeded(unsigned long now) const {
  if (!Running())
    return false;
  if (last_update_time_ == 0)
    return true;
  size_t last_step_index;
  size_t step_index;
  long time_offset;
  GetStep(last_update_time_, &last_step_index, &time_offset);
  if (!GetStep(now, &step_index, &time_offset))
    return true;
  if (last_step_index != step_index)
    return true;
  if (now - last_update_time_ >= steps_[step_index].update_interval)
    return true;
  return false;
}

bool EffectSequence::GetStep(unsigned long now, size_t* step_index, unsigned long* time_offset) const {
  if (!Running())
    halt(F("Not running"));
  *time_offset = now - start_time_;
  *step_index = 0;
  while (*step_index < steps_count_) {
    if (*time_offset < steps_[*step_index].duration) {
      return true;
    }
    *time_offset -= steps_[*step_index].duration;
    *step_index += 1;
  }
  return false;  
}

unsigned long EffectSequence::GetStepDuration(size_t step_index) const {
  return steps_[step_index].duration;
}

void EffectSequence::DisplayUpdated(unsigned long now) {
  if (!Running())
    halt(F("Not running"));
  size_t step_index;
  long time_offset;
  if (GetStep(now, &step_index, &time_offset)) {
    last_update_time_ = now;
    return;
  }
  start_time_ = 0;
  last_update_time_ = 0;
}

