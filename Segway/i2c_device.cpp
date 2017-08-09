#include "i2c_device.h"

#include <Wire.h>

I2CDevice::I2CDevice(byte dev_addr)
    : dev_addr_(dev_addr) {}

int I2CDevice::WriteRegister(byte reg_addr, byte data) {
  int written_bytes;

  Wire.beginTransmission(dev_addr_);
  Wire.write(reg_addr);
  written_bytes = Wire.write(&data, 1);
  Wire.endTransmission();

  return written_bytes;
}

byte I2CDevice::ReadByteRegister(byte reg_addr) {
  byte data = 0;
  Wire.beginTransmission(dev_addr_);
  Wire.write(reg_addr);
  Wire.endTransmission();

  Wire.requestFrom(dev_addr_, (byte)1);

  if (Wire.available()) {
    data = Wire.read();
  }

  return data;
}

byte I2CDevice::ReadRegister(byte reg_addr, byte nbytes, byte *buffer) {
  byte idx = 0;

  Wire.beginTransmission(dev_addr_);
  Wire.write(reg_addr | (1 << 7));
  Wire.endTransmission();

  Wire.requestFrom(dev_addr_, nbytes);

  while(Wire.available()) {
    buffer[idx++] = Wire.read();
  }

  return idx;
}
