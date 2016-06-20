#ifndef TEST_COMMAND_HANDLER_H_
#define TEST_COMMAND_HANDLER_H_

#include "command_handler.h"

class TestCommandHandler : public CommandHandler {
 public:
  TestCommandHandler();
  ~TestCommandHandler() override;

  void HandleButton(int button, bool state) override;
};

#endif  // TEST_COMMAND_HANDLER_H_
