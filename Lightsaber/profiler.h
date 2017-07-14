#ifndef PROFILER_H_
#define PROFILER_H_

class Profiler {
 public:
  Profiler();
  void Start();
  void Record();

 private:
  unsigned long start_time_;
  unsigned long elapsed_time_;
  unsigned long sample_count_;
};

#endif  // PROFILER_H_