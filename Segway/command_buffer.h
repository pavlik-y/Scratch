#ifndef COMMAND_BUFFER_H_
#define COMMAND_BUFFER_H_

#include <Arduino.h>
#include <SoftwareSerial.h>

class CommandBuffer {
public:
  void Setup(SoftwareSerial* bt);
  bool ReadCommand();
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

  const char* command;

private:

  void ProcessInput(char* buffer);

  SoftwareSerial* bt_;

  char buffer_[64];

  char* params[7];
  int param_count;

  int index_;
  bool first_column_;
};

#endif  // COMMAND_BUFFER_H_
