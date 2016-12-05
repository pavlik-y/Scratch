#ifndef EFFECT_SEQUENCE_H_
#define EFFECT_SEQUENCE_H_

class EffectSequence {
 public:
  struct Step {
    unsigned long duration;
    unsigned long update_interval;
  };
  
  EffectSequence(const Step* steps, size_t steps_count);

  void Start(unsigned long now);
  bool Running() const;
  bool DisplayUpdateNeeded(unsigned long now) const;
  bool GetStep(unsigned long now, size_t* step_index, unsigned long* time_offset) const;
  unsigned long GetStepDuration(size_t step_index) const;

  void DisplayUpdated(unsigned long now);
  
 private:
  const Step* const steps_;
  const size_t steps_count_;

  unsigned long start_time_;
  unsigned long last_update_time_;
};
 
#endif  // EFFECT_SEQUENCE_H_
