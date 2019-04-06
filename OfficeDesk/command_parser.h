#ifndef COMMAND_PARSER_H_
#define COMMAND_PARSER_H_

class BLEUart;
class Controller;

class CommandParser {
 public:
  CommandParser(BLEUart* ble, Controller* controller);

  void Tick();

 private:
  bool HandleBlueFruitCommand();
  BLEUart* ble_;
  Controller* controller_;

  unsigned long last_char_time_;
  char buf[10];
  int idx = 0;
};

#endif  // COMMAND_PARSER_H_
