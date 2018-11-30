#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>
#include <Nffs.h>

#include "common.h"
#include "component.h"

class CommandBuffer;

// WrConfig:Motor.Right.MaxVel,100
#define CONFIG_VALUES(F) \
  F(kConfigVersion, "FormatVersion", 1) \
  F(kGyro_Bias, "Gyro.Bias", -40) \
  F(kAccel_BiasY, "Accel.BiasY", -150) \
  F(kAccel_BiasZ, "Accel.BiasZ", -800) \
  F(kSF_Lambda, "SF.Lambda", 0.02) \
  F(kBal_UprightAngle, "Bal.UprightAngle", 0) \
  F(kTiltCtrl_KP, "TiltCtrl.KP", 100) \
  F(kTiltCtrl_KD, "TiltCtrl.KD", 0) \
  F(kMotor_Left_MinVel, "Motor.Left.MinVel", 10) \
  F(kMotor_Left_MaxVel, "Motor.Left.MaxVel", 160) \
  F(kMotor_Left_MinPow, "Motor.Left.MinPow", 0.3) \
  F(kMotor_Left_MaxPow, "Motor.Left.MaxPow", 0.9) \
  F(kMotor_Right_MinVel, "Motor.Right.MinVel", 10) \
  F(kMotor_Right_MaxVel, "Motor.Right.MaxVel", 160) \
  F(kMotor_Right_MinPow, "Motor.Right.MinPow", 0.3) \
  F(kMotor_Right_MaxPow, "Motor.Right.MaxPow", 0.9) \
  F(kMotor_Enabled, "Motor.Enabled", 0) \
  F(kVelCtrl_KP, "VelCtrl.KP", 0) \
  F(kVelCtrl_KI, "VelCtrl.KI", 0) \
  F(kVelCtrl_KD, "VelCtrl.KD", 0) \
  F(kVelCtrl_KL, "VelCtrl.KL", 0) \
  F(kMotor_TurnOffset, "Motor.TurnOffset", 0) \
  F(kPos_SampleInterval, "Pos.SampleInterval", 10000.0) \
  F(kDiagMode, "DiagMode", 0) \

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
