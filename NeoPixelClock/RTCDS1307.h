#ifndef RTCDS1307_H_
#define RTCDS1307_H_

#include <Arduino.h>
#include <Wire.h>

class RTCDS1307 {
public:
  void Setup() {
    Wire.beginTransmission(DS1307_ADDR);
    Wire.write(7);
    Wire.write(0x10);
    Wire.endTransmission();
  }
  
  bool ReadClock() {
    Wire.beginTransmission(DS1307_ADDR);
    Wire.write(0);
    Wire.endTransmission();
    Wire.requestFrom(DS1307_ADDR, (byte)3);
    
    byte buf;
    if (!Wire.available())
      return false;
    buf = Wire.read();
    sec = (buf & 0xf) + (buf >> 4) * 10;
    if (!Wire.available())
      return false;
    buf = Wire.read();
    min = (buf & 0xf) + (buf >> 4) * 10;
    if (!Wire.available())
      return false;
    buf = Wire.read();
    hour = (buf & 0xf) + (buf >> 4) * 10;
    if (Wire.available())
      return false;
    return true;
  }
  
  void SetTime(int hours, int minutes) {
    minutes = ((minutes / 10) << 4) | (minutes % 10);
    hours = ((hours / 10) << 4) | (hours % 10);
    Wire.beginTransmission(DS1307_ADDR);
    Wire.write(0);
    Wire.write((byte)0);
    Wire.write((byte)minutes);
    Wire.write((byte)hours);
    Wire.endTransmission();
    ReadClock();
  }
  
  int sec;
  int min;
  int hour;
  
private:
  static const byte DS1307_ADDR = 0b01101000;
};
#endif
