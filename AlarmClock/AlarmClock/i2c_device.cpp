#include "i2c_device.h"

#include <Wire.h>

I2CDevice::I2CDevice(byte dev_addr)
    : dev_addr_(dev_addr) {}

void I2CDevice::RequestReadRegisters(byte reg_addr, byte read_size) {
  Wire.beginTransmission(dev_addr_);
  Wire.write(reg_addr);
  Wire.endTransmission();

  Wire.requestFrom(dev_addr_, read_size);
}

byte I2CDevice::Read() {
  if (Wire.available()) {
    return Wire.read();
  }
  return 0;
}

bool I2CDevice::Available() {
  return Wire.available();
}

void I2CDevice::Flush() {
  Wire.flush();
}

void I2CDevice::BeginWriteRegisters(byte reg_addr) {
  Wire.beginTransmission(dev_addr_);
  Wire.write(reg_addr);
}

int I2CDevice::Write(byte value) {
  return Wire.write(value);
}

void I2CDevice::EndWriteRegisters() {
  Wire.endTransmission();
}

byte I2CDevice::ReadRegister(byte reg_addr) {
  RequestReadRegisters(reg_addr, 1);
  byte result = Read();
  Flush();
  return result;
}

int I2CDevice::WriteRegister(byte reg_addr, byte data) {
  BeginWriteRegisters(reg_addr);
  int result = Write(data);
  EndWriteRegisters();
  return result;
}
