// Chip datasheet: http://infocenter.nordicsemi.com/pdf/nRF52832_PS_v1.4.pdf
// https://www.adafruit.com/product/3406
// Pinout: https://cdn-learn.adafruit.com/assets/assets/000/046/248/original/microcontrollers_Feather_NRF52_Pinout_v1.2-1.png?1504885794

#include <Arduino.h>
#include <bluefruit.h>
#include <Nffs.h>
#include <SwRotaryEncoder.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include "accel.h"
#include "command_buffer.h"
#include "component_manager.h"
#include "config.h"
#include "diag.h"
#include "fall_detector.h"
#include "gyro.h"
#include "motor.h"
#include "motor_controller.h"
#include "motor_encoder.h"
#include "position.h"
#include "sensor_chip.h"
#include "sensor_fusion.h"
#include "tilt_controller.h"
#include "velocity_controller.h"

// Pins
// const int LEFT_MOTOR_A = 6;
// const int LEFT_MOTOR_B = 7;
// const int LEFT_MOTOR_EN = 9;
// const int RIGHT_MOTOR_A = 8;
// const int RIGHT_MOTOR_B = 11;
// const int RIGHT_MOTOR_EN = 10;
// const int RIGHT_ENCODER_A = 2;
// const int RIGHT_ENCODER_B = 4;
//const int LEFT_ENCODER_A = 3;
//const int LEFT_ENCODER_B = 5;
// const int BLUETOOTH_RX = 12;
// const int BLUETOOTH_TX = 13;
const int GYRO_DATA_READY_PIN = 15;
const int RIGHT_ENCODER_A = 7;
const int RIGHT_ENCODER_B = 16;


//MotorEncoder left_encoder;
// // MotorEncoder right_encoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);
// // MotorDriver motor_driver(LEFT_MOTOR_A, LEFT_MOTOR_B, LEFT_MOTOR_EN,
// //                          RIGHT_MOTOR_A, RIGHT_MOTOR_B, RIGHT_MOTOR_EN);
// SoftwareSerial bt(BLUETOOTH_RX, BLUETOOTH_TX);
SwRotaryEncoder right_encoder;

SensorChip sensors(GYRO_DATA_READY_PIN);

CommandBuffer command_buffer;

ComponentManager component_manager(15);

Version config_version;

Accel accel;
Config config;
ConfigStore config_store;
Diag diag;
FallDetector fall_detector;
Gyro gyro;
MotorController motor_controller;
Position position;
SensorFusion sensor_fusion;
TiltController tilt_controller;
VelocityController velocity_controller;

PidController angle_to_power;

//void ReadLeftEncoder() {
//  left_encoder.Read();
//}

// void ReadRightEncoder() {
//   right_encoder.Read();
// }

void SetupInterrupts() {
  // attachInterrupt(0, ReadRightEncoder, CHANGE);
//  attachInterrupt(1, ReadLeftEncoder, CHANGE);
}

void setup() {
  pinMode(15, INPUT);

  Wire.begin();
  Wire.setClock(400000);

  Serial.begin(115200);
  Serial.println("Restart");

  Bluefruit.begin();
  Nffs.begin();

  right_encoder.begin(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

  sensors.Setup();

  // bt.begin(9600);
  // bt.listen();

  // {PAV} pass valid BLE.
  command_buffer.Setup(&Serial);

  // motor_driver.Setup();
  // motor_driver.SetupTimer1();

  config_store.Setup("/Segway.cfg", Config::GetConfigValuesCount());

  config_version = config.version;

  config.Setup(&config_store);
  component_manager.RegisterComponent(&config);

  Serial.println("Before gyro setup");
  gyro.Setup(&sensors);
  component_manager.RegisterComponent(&gyro);

  accel.Setup(&sensors, &gyro);
  component_manager.RegisterComponent(&accel);

  sensor_fusion.Setup(&gyro, &accel);
  component_manager.RegisterComponent(&sensor_fusion);

  fall_detector.Setup(&sensor_fusion);
  component_manager.RegisterComponent(&fall_detector);

  position.Setup(&right_encoder);
  component_manager.RegisterComponent(&position);

  diag.Setup(&gyro, &accel, &sensor_fusion, &position);
  component_manager.RegisterComponent(&diag);

  velocity_controller.Setup(&position);
  component_manager.RegisterComponent(&velocity_controller);

  // tilt_controller.Setup(&sensor_fusion, &velocity_controller);
  // component_manager.RegisterComponent(&tilt_controller);

  // {PAV} I don't think I need calibration.
  // calibration.Setup(&accel, &gyro, &sensor_fusion, &motor_driver);
  // component_manager.RegisterComponent(&calibration);

  // motor_controller.Setup(
  //     &motor_driver, &tilt_controller, &fall_detector, &calibration);
  // component_manager.RegisterComponent(&motor_controller);

  SetupInterrupts();
}

void ProcessCommand(CommandBuffer& cb) {
  if (strcmp_P(cb.command, PSTR("Echo")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(cb.GetStringParam(0));
    cb.EndResponse();
    return;
  }
  if (component_manager.HandleCommand(cb))
    return;
  cb.BeginResponse();
  cb.WriteValue(cb.command);
  cb.EndResponse();
}

void loop() {
  component_manager.Update();

  if (command_buffer.ReadCommand()) {
    ProcessCommand(command_buffer);
  }
  if ((config_version != config.version) && config.initialized) {
    config_version = config.version;
    component_manager.ReadConfig(&config);
  }
}

