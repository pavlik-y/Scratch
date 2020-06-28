#include <Arduino.h>
#include <Wire.h>

#include "display.h"

namespace {
static const int LED_DRIVER_ADDR = 0x70;

struct DigitDescriptor {
  int segment_count;
  int value_count;
  const byte* value_to_segments;
  const int* segment_indices;
};

const byte kValueToSegments[17] = {
  0b0111111,  // 0: ABCDEF
  0b0000110,  // 1: BC
  0b1011011,  // 2: ABDEG
  0b1001111,  // 3: ABCDG
  0b1100110,  // 4: BCFG
  0b1101101,  // 5: ACDFG
  0b1111101,  // 6: ACDEFG
  0b0000111,  // 7: ABC
  0b1111111,  // 8: ABCDEFG
  0b1101111,  // 9: ABCDFG
  0b1110111,  // A: ABCEFG
  0b1111100,  // b: CDEFG
  0b0111001,  // C: ADEF
  0b1011110,  // d: BCDEG
  0b1111001,  // E: ADEFG
  0b1110001,  // F: AEFG
  0b0000000,
};

const byte kValueToSegmentsLeftDigit[] = {
  0b00,
  0b11,
};

const DigitDescriptor digit_descriptors[] = {
  {7, 17, kValueToSegments, (const int[]){0x54,0x34,0x4,0x24,0x64,0x44,0x14}},
  {7, 17, kValueToSegments, (const int[]){0x53,0x33,0x3,0x23,0x63,0x43,0x13}},
  {7, 17, kValueToSegments, (const int[]){0x51,0x31,0x1,0x21,0x61,0x41,0x11}},
  {2, 2, kValueToSegmentsLeftDigit, (const int[]){0x30, 0x0}},
};

const int kStatusBitIndices[9] = {
  0x50, 0x40, 0x60, 0x20, 0x52, 0x42, 0x62, 0x22, 0x12
};

const int kColonIndices[2] = {
  0x2, 0x32
};
}  // namespace

Display::Display() = default;

void Display::Setup() {
  // Wait for 1ms after power-on to allow completion of the reset action.
  delay(2);

  Wire.beginTransmission(LED_DRIVER_ADDR);
  // System setup register: enable oscillator.
  Wire.write(0b00100001);
  Wire.endTransmission();

  Wire.beginTransmission(LED_DRIVER_ADDR);
  // Display setup register: display on.
  Wire.write(0b10000001);
  Wire.endTransmission();

  Show();
}

void Display::Show() {
  Wire.beginTransmission(LED_DRIVER_ADDR);
  // Display memory starting from offset 0.
  Wire.write(0);
  for (int i = 0; i < 16; i++)
    Wire.write(display_ram_[i]);
  Wire.endTransmission();
}

void Display::SetDigit(int digit, int value) {
  const DigitDescriptor* dd = digit_descriptors + digit;
  // if (value >= dd->value_count) halt();

  byte segments = dd->value_to_segments[value];
  for (int i = 0; i < dd->segment_count; i++) {
    if (segments & (1 << i)) {
      SetBit(dd->segment_indices[i]);
    } else {
      ClearBit(dd->segment_indices[i]);
    }
  }
}

void Display::SetStatusBit(int bit, bool value) {
  if (value)
    SetBit(kStatusBitIndices[bit]);
  else
    ClearBit(kStatusBitIndices[bit]);
}

void Display::SetColon(bool value) {
  if (value) {
    SetBit(kColonIndices[0]);
    SetBit(kColonIndices[1]);
  } else {
    ClearBit(kColonIndices[0]);
    ClearBit(kColonIndices[1]);
  }
}

void Display::SetDimLevel(int dim_level) {
  Wire.beginTransmission(LED_DRIVER_ADDR);
  Wire.write(0b11100000 | dim_level);
  Wire.endTransmission();
}

void Display::ReadKeys() {
  Wire.beginTransmission(LED_DRIVER_ADDR);
  // Display setup register: display on.
  Wire.write(0x40);
  Wire.endTransmission();
  Wire.requestFrom(LED_DRIVER_ADDR, sizeof(key_ram_));
  for (int i = 0; i < sizeof(key_ram_); i++) {
    key_ram_[i] = Wire.read();
  }
}

bool Display::GetKeyState(int key) {
  return (key_ram_[key >> 3] & (1 << (key & 0x7))) != 0;
}

void Display::SetBit(int bit_index) {
  int byte_index = bit_index / 8;
  byte bit_mask = 1 << (bit_index % 8);
  display_ram_[byte_index] |= bit_mask;
}

void Display::ClearBit(int bit_index) {
  int byte_index = bit_index / 8;
  byte bit_mask = 1 << (bit_index % 8);
  display_ram_[byte_index] &= ~bit_mask;
}