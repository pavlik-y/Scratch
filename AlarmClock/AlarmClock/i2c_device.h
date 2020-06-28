#ifndef I2C_DEVICE_H_
#define I2C_DEVICE_H_

#include <Arduino.h>

class I2CDevice {
public:
  I2CDevice(byte dev_addr);

  void RequestReadRegisters(byte reg_addr, byte read_size);
  byte Read();
  bool Available();
  void Flush();

  void BeginWriteRegisters(byte reg_addr);
  int Write(byte value);
  void EndWriteRegisters();

  byte ReadRegister(byte reg_addr);
  int WriteRegister(byte reg_addr, byte data);

private:
  byte dev_addr_;
};

#endif  // I2C_DEVICE_H_