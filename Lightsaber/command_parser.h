#ifndef COMMAND_PARSER_H_
#define COMMAND_PARSER_H_

class Adafruit_BLE;
class CommandHandler;

class CommandParser {
 public:
  CommandParser(Adafruit_BLE* ble, CommandHandler* command_handler);

  void Init();
  void Tick();

 private:
  bool HandleBlueFruitCommand();
  bool HandleLightsaberCommand();
  bool HandleLightsaberDiagnostic();
  Adafruit_BLE* ble_;
  CommandHandler* command_handler_;

  unsigned long last_check_time_;
};

#endif  // COMMAND_PARSER_H_
