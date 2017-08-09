#ifndef I2C_DEVICE_H_
#define I2C_DEVICE_H_

#include <Arduino.h>

class I2CDevice {
public:
  I2CDevice(byte dev_addr);

  void SetAddr(byte dev_addr);
  int WriteRegister(byte reg_addr, byte data);

  byte ReadByteRegister(byte reg_addr);
  byte ReadRegister(byte reg_addr, byte nbytes, byte *buffer);
private:
  byte dev_addr_;
};

#endif  // I2C_DEVICE_H_