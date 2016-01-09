#ifndef command_buffer_h_
#define command_buffer_h_

#include <Arduino.h>
#include <SoftwareSerial.h>

class CommandBuffer {
public:
  void Setup(SoftwareSerial* bt) {
    bt_ = bt;
    index_ = 0;
  }

  bool ReadCommand() {
    while(bt_->available()) {
      buffer_[index_] = (char) bt_->read();
      if (buffer_[index_] == '\r')
        continue;
      if (buffer_[index_] == '\n') {
        buffer_[index_] = '\0';
        ProcessInput(buffer_);
        index_ = 0;
        return true;
      }
      if (index_ < 63)
        ++index_;
    }
    return false;
  }

  void ProcessInput(char* buffer) {
    command = strtok(buffer, ":");
    for (param_count = 0; ; ++param_count) {
      params[param_count] = strtok(NULL, ",");
      if (params[param_count] == NULL)
        break;
    }
  }

  const char* GetStringParam(int index) const {
    return params[index];
  }

  int GetIntParam(int index) const {
    return atol(GetStringParam(index));
  }

  float GetFloatParam(int index) const {
    return atof(GetStringParam(index));
  }

  void BeginResponse() {
    bt_->print("RSP:");
    first_column_ = true;
  }

  void WriteValue(int value) {
    if (!first_column_)
      bt_->print(",");
    bt_->print(value);
    first_column_ = false;
  }

  void WriteValue(long value) {
    if (!first_column_)
      bt_->print(",");
    bt_->print(value);
    first_column_ = false;
  }

  void WriteValue(double value) {
    if (!first_column_)
      bt_->print(",");
    bt_->print(value, 4);
    first_column_ = false;
  }

  void WriteValue(const String& value) {
    if (!first_column_)
      bt_->print(",");
    bt_->print(value);
    first_column_ = false;
  }

  void WriteValue(const char* value) {
    if (!first_column_)
      bt_->print(",");
    bt_->print(value);
    first_column_ = false;
  }

  void EndResponse() {
    bt_->println();
  }

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
#endif
