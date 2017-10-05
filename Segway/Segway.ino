#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include "accel.h"
#include "calibration.h"
#include "command_buffer.h"
#include "component_manager.h"
#include "config.h"
#include "fall_detector.h"
#include "gyro.h"
#include "motor.h"
#include "motor_controller.h"
#include "motor_encoder.h"
#include "position.h"
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


//MotorEncoder left_encoder;
// // MotorEncoder right_encoder(RIGHT_ENCODER_A, RIGHT_ENCODER_B);
// // MotorDriver motor_driver(LEFT_MOTOR_A, LEFT_MOTOR_B, LEFT_MOTOR_EN,
// //                          RIGHT_MOTOR_A, RIGHT_MOTOR_B, RIGHT_MOTOR_EN);
// SoftwareSerial bt(BLUETOOTH_RX, BLUETOOTH_TX);
CommandBuffer command_buffer;

ComponentManager component_manager(15);

Version config_version;

Accel accel;
Calibration calibration;
Config config;
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
  Wire.begin();
  Serial.begin(9600);
  Serial.println("Restart");

  // bt.begin(9600);
  // bt.listen();

  // {PAV} pass valid BLE.
  // command_buffer.Setup(&bt);

  // right_encoder.Setup();

  // motor_driver.Setup();
  // motor_driver.SetupTimer1();

  config_version = config.version;

  config.Setup();
  component_manager.RegisterComponent(&config);

  gyro.Setup();
  component_manager.RegisterComponent(&gyro);

  accel.Setup(&gyro);
  component_manager.RegisterComponent(&accel);

  // position.Setup(&right_encoder);
  // component_manager.RegisterComponent(&position);

  sensor_fusion.Setup(&gyro, &accel);
  component_manager.RegisterComponent(&sensor_fusion);

  fall_detector.Setup(&sensor_fusion);
  component_manager.RegisterComponent(&fall_detector);

  velocity_controller.Setup(&position);
  component_manager.RegisterComponent(&velocity_controller);

  tilt_controller.Setup(&sensor_fusion, &velocity_controller);
  component_manager.RegisterComponent(&tilt_controller);

  // calibration.Setup(&accel, &gyro, &sensor_fusion, &motor_driver);
  // component_manager.RegisterComponent(&calibration);

  // motor_controller.Setup(
  //     &motor_driver, &tilt_controller, &fall_detector, &calibration);
  // component_manager.RegisterComponent(&motor_controller);

//  diag.Setup(&sensor_fusion, &balancer);
//  component_manager.RegisterComponent(&diag);

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

