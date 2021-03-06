// Chip datasheet: http://infocenter.nordicsemi.com/pdf/nRF52832_PS_v1.4.pdf
// https://www.adafruit.com/product/3406
// Pinout: https://cdn-learn.adafruit.com/assets/assets/000/046/248/original/microcontrollers_Feather_NRF52_Pinout_v1.2-1.png?1504885794

#include <Arduino.h>

#include <bluefruit.h>
#include <HardwarePWM.h>
#include <Nffs.h>
#include <SwRotaryEncoder.h>
#include <SoftwareSerial.h>
#include <Wire.h>

#include "accel.h"
#include "ble_remote.h"
#include "command_buffer.h"
#include "component_manager.h"
#include "config.h"
#include "diag.h"
#include "fall_detector.h"
#include "gyro.h"
#include "motor_driver.h"
#include "motor_controller.h"
#include "motor_encoder.h"
#include "position.h"
#include "sensor_chip.h"
#include "sensor_fusion.h"
#include "tilt_controller.h"
#include "velocity_controller.h"

// Pins
const int GYRO_DATA_READY_PIN = 27;

const int RIGHT_ENCODER_A = 7;
const int RIGHT_ENCODER_B = 11;
const int LEFT_ENCODER_A = 15;
const int LEFT_ENCODER_B = 16;

const int LEFT_MOTOR_A = 28;
const int LEFT_MOTOR_B = 29;
const int LEFT_MOTOR_EN = 2;
const int RIGHT_MOTOR_A = 5;
const int RIGHT_MOTOR_B = 4;
const int RIGHT_MOTOR_EN = 3;


MotorDriver motor_driver(&HwPWM0,
    LEFT_MOTOR_A, LEFT_MOTOR_B, LEFT_MOTOR_EN,
    RIGHT_MOTOR_A, RIGHT_MOTOR_B, RIGHT_MOTOR_EN);

BLEUart ble;

SwRotaryEncoder right_encoder;
SwRotaryEncoder left_encoder;

SensorChip sensors(GYRO_DATA_READY_PIN);

CommandBuffer command_buffer;

ComponentManager component_manager(15);

Version config_version;

Accel accel;
BleRemote ble_remote;
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

void setup() {

  Wire.begin();
  Wire.setClock(400000);

  Serial.begin(115200);
  Serial.println("Restart");

  Bluefruit.begin();
  Nffs.begin();
  ble.begin();

  left_encoder.begin(LEFT_ENCODER_A, LEFT_ENCODER_B);
  right_encoder.begin(RIGHT_ENCODER_A, RIGHT_ENCODER_B);
  motor_driver.Setup();

  Serial.println("Before sensors Setup");
  pinMode(GYRO_DATA_READY_PIN, INPUT);
  sensors.Setup();
  Serial.println("After sensors Setup");


  // bt.begin(9600);
  // bt.listen();

  // {PAV} pass valid BLE.
  command_buffer.Setup(&Serial);

  config_store.Setup("/Segway.cfg", Config::GetConfigValuesCount());

  config_version = config.version;

  Serial.println("Before config Setup");
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

  tilt_controller.Setup(&sensor_fusion);
  component_manager.RegisterComponent(&tilt_controller);

  motor_controller.Setup(&motor_driver, &tilt_controller, &fall_detector);
  component_manager.RegisterComponent(&motor_controller);

  position.Setup(&left_encoder, &right_encoder);
  component_manager.RegisterComponent(&position);

  velocity_controller.Setup(&position, &tilt_controller);
  component_manager.RegisterComponent(&velocity_controller);

  ble_remote.Setup(&ble, &velocity_controller, &motor_controller);
  component_manager.RegisterComponent(&ble_remote);

  // power_adjuster.Setup(&position, &tilt_controller, &motor_controller);
  // component_manager.RegisterComponent(&power_adjuster);

  Serial.println("Before diag Setup");
  diag.Setup(&gyro, &accel, &sensor_fusion,
      &fall_detector, &tilt_controller, &motor_controller,
      &left_encoder, &right_encoder, &position, &velocity_controller);
  component_manager.RegisterComponent(&diag);

  SetupBle();
  Serial.println("Setup done");
}

void SetupBle() {
  Bluefruit.setName("Segway");
  Bluefruit.setConnectCallback(ConnectCallback);
  Bluefruit.setDisconnectCallback(DisconnectCallback);
  Bluefruit.Advertising.addService(ble);
  Bluefruit.ScanResponse.addName();
  Bluefruit.Advertising.restartOnDisconnect(true);
  Bluefruit.Advertising.setInterval(32, 244); // in unit of 0.625 ms
  Bluefruit.Advertising.setFastTimeout(30); // number of seconds in fast mode
  // 0 = Don't stop advertising after n seconds
  Bluefruit.Advertising.start(0);
}

void ConnectCallback(uint16_t conn_handle) {
  Serial.println("Connected");
  ble_remote.SetConnected(true);
}

void DisconnectCallback(uint16_t conn_handle, uint8_t reason)
{
  ble_remote.SetConnected(false);
  Serial.println("Disconnected");
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

