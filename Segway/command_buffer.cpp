#include "command_buffer.h"

void CommandBuffer::Setup(SoftwareSerial* bt) {
  bt_ = bt;
  index_ = 0;
}

bool CommandBuffer::ReadCommand() {
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

void CommandBuffer::ProcessInput(char* buffer) {
  command = strtok(buffer, ":");
  for (param_count = 0; ; ++param_count) {
    params[param_count] = strtok(NULL, ",");
    if (params[param_count] == NULL)
      break;
  }
}

const char* CommandBuffer::GetStringParam(int index) const {
  return params[index];
}

int CommandBuffer::GetIntParam(int index) const {
  return atol(GetStringParam(index));
}

float CommandBuffer::GetFloatParam(int index) const {
  return atof(GetStringParam(index));
}

void CommandBuffer::BeginResponse() {
  bt_->print("RSP:");
  first_column_ = true;
}

void CommandBuffer::WriteValue(int value) {
  if (!first_column_)
    bt_->print(",");
  bt_->print(value);
  first_column_ = false;
}

void CommandBuffer::WriteValue(long value) {
  if (!first_column_)
    bt_->print(",");
  bt_->print(value);
  first_column_ = false;
}

void CommandBuffer::WriteValue(double value) {
  if (!first_column_)
    bt_->print(",");
  bt_->print(value, 4);
  first_column_ = false;
}

void CommandBuffer::WriteValue(const String& value) {
  if (!first_column_)
    bt_->print(",");
  bt_->print(value);
  first_column_ = false;
}

void CommandBuffer::WriteValue(const char* value) {
  if (!first_column_)
    bt_->print(",");
  bt_->print(value);
  first_column_ = false;
}

void CommandBuffer::EndResponse() {
  bt_->println();
}
