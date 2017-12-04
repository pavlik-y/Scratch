#include "command_buffer.h"

void CommandBuffer::Setup(Stream* stream) {
  stream_ = stream;
  index_ = 0;
}

bool CommandBuffer::ReadCommand() {
  while(stream_->available()) {
    buffer_[index_] = (char) stream_->read();
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
  stream_->print("RSP:");
  first_column_ = true;
}


template <>
void CommandBuffer::WriteValue(double value) {
  stream_->printf("%s%.4f", first_column_? "": ",", value);
  // if (!first_column_)
  //   stream_->print(",");
  // stream_->print(value, 4);
  first_column_ = false;
}

void CommandBuffer::EndResponse() {
  stream_->println();
}
