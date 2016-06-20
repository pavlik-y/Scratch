#ifndef COMMAND_HANDLER_H_
#define COMMAND_HANDLER_H_

class CommandHandler {
 public:
  virtual ~CommandHandler();
  virtual void HandleButton(int button, bool state);
};

#endif
