#include "time_display.h"

#include "profiler.h"

TimeDisplay::TimeDisplay(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor,
    RTC_PCF8523* rtc)
    : MotionDisplay(strip, sensor),
      rtc_(rtc) {
  SetPatternLen(8);
}

void TimeDisplay::OnSequenceStart() {
  time_to_display_ = rtc_->now();
}

void TimeDisplay::ShowPattern(int idx, Adafruit_NeoPixel* strip) {
  if (idx < 4) {
    ShowDigitPattern(time_to_display_.second() % 10, idx % 4, strip);
  } else {
    ShowDigitPattern(time_to_display_.second() / 10, idx % 4, strip);
  }
}

//   1
// 2   3
//   4
// 5   6
//   7
const PROGMEM byte digit_segments[] = {
  0x77, // 0: 111 0111
  0x44, // 1: 010 0100
  0x5d, // 2: 101 1101
  0x6d, // 3: 110 1101
  0x2e, // 4: 010 1110
  0x6b, // 5: 110 1011
  0x7b, // 6: 111 1011
  0x25, // 7: 010 0101
  0x7f, // 8: 111 1111
  0x6f  // 9: 110 1111
};

const PROGMEM uint16_t segment_patterns[] = {
  0x0001, // 1: 1
  0x001f, // 2: 1-5
  0x001f, // 3: 1-5
  0x0010, // 4: 5
  0x01f0, // 5: 5-9
  0x01f0, // 6: 5-9
  0x0100 // 7: 9
};

const PROGMEM byte frame_segments[] = {
  0x5b, // 0: 101 1011
  0x49, // 1: 100 1001
  // 0x49, // 2: 100 1001
  // 0x49, // 3: 100 1001
  0x5b, // 4: 101 1011
  0x00  // 5: 000 0000
};

void TimeDisplay::ShowDigitPattern(int digit, int idx,
    Adafruit_NeoPixel* strip) {
  byte ds = pgm_read_byte(digit_segments + digit);
  byte fs = pgm_read_byte(frame_segments + idx);
  byte segments = ds & fs ;

  uint16_t pattern = 0;
  for (int i = 0; i < 7; i++) {
    if (segments & (1 << i))
      pattern |= pgm_read_word(segment_patterns + i);
  }

  for (int i = 0; i < sizeof(pattern)*8; i++) {
    byte v = 0;
    if (pattern & (1 << i))
      v = 255;
    strip->setPixelColor(10 + i, 0, v, 0);
  }
  strip->show();
}