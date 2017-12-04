#ifndef COMMAND_BUFFER_H_
#define COMMAND_BUFFER_H_

#include <Arduino.h>
#include <Stream.h>

class CommandBuffer {
public:
  void Setup(Stream* stream);
  bool ReadCommand();
  const char* GetStringParam(int index) const;
  int GetIntParam(int index) const;
  float GetFloatParam(int index) const;

  void BeginResponse();
  template <typename T> void WriteValue(T value);
  void EndResponse();

  // Parsed command.
  const char* command;

private:
  void ProcessInput(char* buffer);

  Stream* stream_;

  char buffer_[64];

  // Parsed params.
  char* params[7];
  int param_count;

  // Position in |buffer_| to write next symbol.
  int index_;

  // Controls whether to write ',' before value in Write###.
  bool first_column_;
};

template <typename T>
void CommandBuffer::WriteValue(T value) {
  if (!first_column_)
    stream_->print(",");
  stream_->print(value);
  first_column_ = false;
}

#endif  // COMMAND_BUFFER_H_
