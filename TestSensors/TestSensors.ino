#include <Arduino.h>

#include <Wire.h>

#include "i2c_device.h"

void halt(const char* msg) {
  Serial.println(msg);
  while(true) {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
  }
}

class Gyro {
 public:
  Gyro();
  void Setup();
  bool DataAvailable();
  void ReadAccelData();
  void ReadGyroData();

  int16_t a_x;
  int16_t a_y;
  int16_t a_z;

  int16_t g_x;
  int16_t g_y;
  int16_t g_z;


 private:
  I2CDevice device_;
};

Gyro::Gyro()
    : device_(0x6b) {}

void Gyro::Setup() {
  uint8_t who_am_i = device_.ReadByteRegister(0xf); // WHO_AM_I
  // Serial.println(who_am_i, BIN);
  if (who_am_i != 0x68)
    halt("Gyro not detected");
  device_.WriteRegister(0x10, 0b01101000); // CTRL_REG1_G
  // Serial.print("Gyro ctrl: ");
  // Serial.println(device_.ReadByteRegister(0x10), BIN);
  device_.WriteRegister(0x0C, 0b00000010);
  // Serial.println("FIFO_CTRL");
  // Serial.println(device_.ReadByteRegister(0x2e), BIN);
  device_.WriteRegister(0x2e, 0b00100000);
  // Serial.println(device_.ReadByteRegister(0x2e), BIN);
}

bool Gyro::DataAvailable() {
  // uint8_t stat = device_.ReadByteRegister(0x17);
  // Serial.println(stat, BIN);
  // return (stat & 0b10) != 0;
  return digitalRead(15) == HIGH;
}

void Gyro::ReadAccelData() {
  uint8_t buffer[6];
  device_.ReadRegister(0x28, 6, buffer); // OUT_X_XL
  int16_t* p = (int16_t*)buffer;
  a_x = p[0];
  a_y = p[1];
  a_z = p[2];
}

void Gyro::ReadGyroData() {
  uint8_t buffer[6];
  device_.ReadRegister(0x18, 6, buffer); // OUT_X_G
  int16_t* p = (int16_t*)buffer;
  g_x = p[0];
  g_y = p[1];
  g_z = p[2];
}

Gyro gyro;

void setup() {
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(15, INPUT);
  // Serial.println("Before Wire.begin");
  Wire.begin();
  // Serial.println("Restarted");
  gyro.Setup();
}

int sample_count = 0;

void loop() {
  while(gyro.DataAvailable()) {
    gyro.ReadGyroData();
    gyro.ReadAccelData();
    sample_count++;
    Serial.printf("%d,%d,%d,0\n", gyro.g_x, gyro.g_y, gyro.g_z);
  }
}