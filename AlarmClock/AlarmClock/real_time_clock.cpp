#include "real_time_clock.h"

#include <Arduino.h>
#include <Wire.h>

RealTimeClock::RealTimeClock()
    : rtc_(0b1101000) {}

void RealTimeClock::Setup() {
  // Control_1: 24 hr mode, second interrupt enabled
  rtc_.WriteRegister(0, 0b00000100);
  // Control_3: Battery switchover enabled in standard mode.
  rtc_.WriteRegister(2, 0b00000000);
}

void RealTimeClock::Read() {
  byte buffer[3];
  // Read 3 bytes starting from register 3. Sec, min,hours.
  byte value;
  rtc_.RequestReadRegisters(3, 3);
  value = rtc_.Read();
  seconds = (value & 0xf) + ((value >> 4) & 0x7) * 10;
  value = rtc_.Read();
  minutes = (value & 0xf) + (value >> 4) * 10;
  value = rtc_.Read();
  hours = (value & 0xf) + (value >> 4) * 10;;
}

void RealTimeClock::Adjust() {
  rtc_.BeginWriteRegisters(3);
  rtc_.Write(((seconds / 10) << 4) | (seconds % 10));
  rtc_.Write(((minutes / 10) << 4) | (minutes % 10));
  rtc_.Write(((hours / 10) << 4) | (hours % 10));
  rtc_.EndWriteRegisters();
}
