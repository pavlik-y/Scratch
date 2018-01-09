#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>
#include <Nffs.h>

#include "common.h"
#include "component.h"

class CommandBuffer;

#define CONFIG_VALUES(F) \
  F(kConfigVersion, "FormatVersion", 2.0) \
  F(kGyro_Bias, "Gyro.Bias", -95) \
  F(kAccel_BiasX, "Accel.BiasX", 0) \
  F(kAccel_BiasZ, "Accel.BiasZ", 0) \
  F(kSF_Lambda, "SF.Lambda", 0) \
  F(kBal_UprightAngle, "Bal.UprightAngle", 0) \
  F(kTiltCtrl_KP, "TiltCtrl.KP", 0) \
  F(kTiltCtrl_KI, "TiltCtrl.KI", 0) \
  F(kTiltCtrl_KD, "TiltCtrl.KD", 0) \
  F(kTiltCtrl_KL, "TiltCtrl.KL", 0) \
  F(kMotor_LeftZero, "Motor.LeftZero", 0) \
  F(kMotor_LeftOne, "Motor.LeftOne", 0) \
  F(kMotor_RightZero, "Motor.RightZero", 0) \
  F(kMotor_RightOne, "Motor.RightOne", 0) \
  F(kVelCtrl_KP, "VelCtrl.KP", 0) \
  F(kVelCtrl_KI, "VelCtrl.KI", 0) \
  F(kVelCtrl_KD, "VelCtrl.KD", 0) \
  F(kVelCtrl_KL, "VelCtrl.KL", 0) \
  F(kMotor_Enabled, "Motor.Enabled", 0) \
  F(kMotor_TurnOffset, "Motor.TurnOffset", 0) \
  F(kPos_SampleInterval, "Pos.SampleInterval", 0) \
  F(kPos_Lambda, "Pos.Lambda", 0) \

class ConfigStore {
public:
  ConfigStore();

  void Setup(const char* file_name, int values_count);
  bool IsValid();
  float ReadValue(int index);
  void WriteValue(int index, float value);

private:
  int values_count_;
  NffsFile config_file_;
};

class Config : public Component {
public:
  static int GetConfigValuesCount();

  Config();
  void Setup(ConfigStore* config_store);

  void Update() override;
  bool HandleCommand(CommandBuffer& cb) override;

  float ReadFloat_P(const char* name);

  bool initialized = false;
  Version version = 0;

private:
  int FindEntry(const char* name);
  int FindEntry_P(const char* name);
  float ReadFloat(int i);
  void WriteFloat(int i, double value);
  void ResetToDefaultValues();
  float GetDefaultValue(int i);

  static const char* const entries_[];

  ConfigStore* config_store_ = nullptr;
};

#define DECLARE_CONFIG_VALUE(const_name, config_name, default_value) \
  extern const char const_name[] PROGMEM;

CONFIG_VALUES(DECLARE_CONFIG_VALUE)

#endif
