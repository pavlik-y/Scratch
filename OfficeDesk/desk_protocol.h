#ifndef DESK_PROTOCOL_H_
#define DESK_PROTOCOL_H_

#include <Arduino.h>

class Controller;

// http://www.plantuml.com/plantuml/uml/SoWkIImgAStDuOhMYbNGrRLJy7JnSUK24M1UaAkZIwI2bK9EVZaNXArRMW1L1HJEo4pFW8ddfAQL06LyF8D3NFqy_Tp1klBI5SeJCqiKD19BIXAJCnGoyqfIYufA2qeqkU0AeKO61pWwmyq4SI0ky7KCyeOw0CJ4hHZXgkB4t8ASL8MIpDnKXVoor2AuHZnA75BpKe310000

class DeskProtocol {
 public:
  DeskProtocol(Controller* controller);
  void Setup(int pin, int pin_out);
  void Tick();

 private:
  enum class State {
    IDLE,
    READY,
    LISTENING,
    MARK,
    SPACE
  };

  static void InterruptHandler();

  void SetState(State state);
  void HandleEdge(uint32_t now);
  void RecordSample(uint32_t now);
  uint32_t DecodeWord();

  constexpr static uint32_t kSampleCapacity = 50;

  static DeskProtocol* s_current_reader_;

  Controller* controller_;
  int pin_;
  int pin_out_;
  State state_ = State::IDLE;
  uint32_t ready_time_ = 0;
  uint32_t samples_[kSampleCapacity];
  int sample_size_ = 0;
};

#endif  // DESK_PROTOCOL_H_