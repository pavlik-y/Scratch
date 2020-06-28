#ifndef DISPLAY_H_
#define DISPLAY_H_


class Display {
 public:
  Display();

  void Setup();

  void Show();
  void SetDigit(int digit, int value);
  void SetStatusBit(int bit, bool value);
  void SetColon(bool value);

  void SetDimLevel(int dim_level);

  void ReadKeys();
  bool GetKeyState(int key);

 private:
  void SetBit(int bit_index);
  void ClearBit(int bit_index);

  byte display_ram_[16] = {};
  byte key_ram_[6] = {};
};

#endif  // DISPLAY_H_