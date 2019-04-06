#ifndef CONTROLLER_H_
#define CONTROLLER_H_

class DeskController;

class Controller {
 public:
  Controller(DeskController* desk_controller);
  void ButtonPressed(int button);
  void ButtonReleased(int button);

 private:
  DeskController* desk_controller_;
};

#endif  // CONTROLLER_H_