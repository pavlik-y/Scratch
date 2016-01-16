#include <Arduino.h>
#include <EEPROM.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <math.h>

#include <IRremote.h>

#include "accel.h"
#include "calibration.h"
#include "command_buffer.h"
#include "config.h"
#include "component.h"
#include "fall_detector.h"
#include "gyro.h"
#include "I2CDevice.h"
#include "ir.h"
#include "motor.h"
#include "motor_controller.h"
#include "pid.h"
#include "position.h"
#include "potentiometer.h"
#include "sensor_fusion.h"
#include "tilt_controller.h"
#include "velocity_controller.h"

// Pins
const int LEFT_MOTOR_A = 6;
const int LEFT_MOTOR_B = 7;
const int LEFT_MOTOR_EN = 9;
const int RIGHT_MOTOR_A = 8;
const int RIGHT_MOTOR_B = 11;
const int RIGHT_MOTOR_EN = 10;
const int RIGHT_ENCODER_A = 2;
const int RIGHT_ENCODER_B = 4;
//const int LEFT_ENCODER_A = 3;
//const int LEFT_ENCODER_B = 5;
const int BLUETOOTH_RX = 12;
const int BLUETOOTH_TX = 13;
const int IR_IN = 3;


//MotorEncoder left_encoder;
MotorEncoder right_encoder;
MotorDriver motor_driver;


ComponentManager component_manager(15);

Version config_version;

Accel accel;
Calibration calibration;
Config config;
FallDetector fall_detector;
Gyro gyro;
MotorController motor_controller;
Position position;
Potentiometer potentiometer;
SensorFusion sensor_fusion;

IR ir;

TiltController tilt_controller;
VelocityController velocity_controller;

SoftwareSerial bt(BLUETOOTH_RX, BLUETOOTH_TX);
CommandBuffer command_buffer;

PidController position_to_velocity;
PidController velocity_to_angle;
PidController angle_to_power;




//void ReadLeftEncoder() {
//  left_encoder.Read();
//}

void ReadRightEncoder() {
  right_encoder.Read();
}

void SetupInterrupts() {
  attachInterrupt(0, ReadRightEncoder, CHANGE);
//  attachInterrupt(1, ReadLeftEncoder, CHANGE);
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  Serial.println("Restart");
  bt.begin(9600);
  bt.listen();

  command_buffer.Setup(&bt);
//  left_encoder.Setup(LEFT_ENCODER_A, LEFT_ENCODER_B);
  right_encoder.Setup(RIGHT_ENCODER_A, RIGHT_ENCODER_B);

  motor_driver.Setup(
      LEFT_MOTOR_A, LEFT_MOTOR_B, LEFT_MOTOR_EN,
      RIGHT_MOTOR_A, RIGHT_MOTOR_B, RIGHT_MOTOR_EN);
  motor_driver.SetupTimer1();
  
  
  config_version = config.version;
  
  config.Setup();
  component_manager.RegisterComponent(&config);
  potentiometer.Setup();
  component_manager.RegisterComponent(&potentiometer);
  ir.Setup(IR_IN);
  component_manager.RegisterComponent(&ir);
  gyro.Setup();
  component_manager.RegisterComponent(&gyro);
  accel.Setup(&gyro);
  component_manager.RegisterComponent(&accel);
//  position.Setup(&left_encoder, &right_encoder);
  position.Setup(&right_encoder);
  component_manager.RegisterComponent(&position);
  sensor_fusion.Setup(&gyro, &accel);
  component_manager.RegisterComponent(&sensor_fusion);
  fall_detector.Setup(&sensor_fusion);
  component_manager.RegisterComponent(&fall_detector);
  velocity_controller.Setup(&velocity_to_angle, &position, &potentiometer, &ir);
  component_manager.RegisterComponent(&velocity_controller);
  tilt_controller.Setup(&sensor_fusion, &angle_to_power, &velocity_controller, &potentiometer);
  component_manager.RegisterComponent(&tilt_controller);
  calibration.Setup(&accel, &gyro, &sensor_fusion, &motor_driver);
  component_manager.RegisterComponent(&calibration);
  motor_controller.Setup(&motor_driver, &tilt_controller, &fall_detector, &calibration, &ir);
  component_manager.RegisterComponent(&motor_controller);
  
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
  component_manager.Update( );

  if (command_buffer.ReadCommand()) {
    ProcessCommand(command_buffer);
  }
  if ((config_version != config.version) && config.initialized) {
    config_version = config.version;
    component_manager.ReadConfig(&config);
  }
}
