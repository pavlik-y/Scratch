#ifndef COMMAND_BUFFER_H_
#define COMMAND_BUFFER_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

class CommandBuffer {
public:
  void Setup(SoftwareSerial* bt);
  bool ReadCommand();
  void ProcessInput(char* buffer);
  const char* GetStringParam(int index) const;
  int GetIntParam(int index) const;
  float GetFloatParam(int index) const;
  void BeginResponse();
  void WriteValue(int value);
  void WriteValue(long value);
  void WriteValue(double value);
  void WriteValue(const String& value);
  void WriteValue(const char* value);
  void EndResponse();

  char* command;
  char* params[7];
  int param_count;

private:
  SoftwareSerial* bt_;
  char buffer_[64];
  int index_;
  bool first_column_;
  int commas_[7];
};

#endif  // COMMAND_BUFFER_H_
