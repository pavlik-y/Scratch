#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>

#include "common.h"
#include "component.h"

class CommandBuffer;

#define CONFIG_VALUES(F) \
  F(kConfigHash, "ConfigHash") \
  F(kGyro_Bias, "Gyro.Bias") \
  F(kAccel_BiasX, "Accel.BiasX") \
  F(kAccel_BiasZ, "Accel.BiasZ") \
  F(kSF_Lambda, "SF.Lambda") \
  F(kBal_UprightAngle, "Bal.UprightAngle") \
  F(kTiltCtrl_KP, "TiltCtrl.KP") \
  F(kTiltCtrl_KI, "TiltCtrl.KI") \
  F(kTiltCtrl_KD, "TiltCtrl.KD") \
  F(kTiltCtrl_KL, "TiltCtrl.KL") \
  F(kMotor_LeftZero, "Motor.LeftZero") \
  F(kMotor_LeftOne, "Motor.LeftOne") \
  F(kMotor_RightZero, "Motor.RightZero") \
  F(kMotor_RightOne, "Motor.RightOne") \
  F(kVelCtrl_KP, "VelCtrl.KP") \
  F(kVelCtrl_KI, "VelCtrl.KI") \
  F(kVelCtrl_KD, "VelCtrl.KD") \
  F(kVelCtrl_KL, "VelCtrl.KL") \
  F(kMotor_Enabled, "Motor.Enabled") \
  F(kMotor_TurnOffset, "Motor.TurnOffset") \
  F(kVacant3, "Vacant3") \
  F(kPos_SampleInterval, "Pos.SampleInterval") \
  F(kPos_Lambda, "Pos.Lambda") \

class Config : public Component {
public:
  static const size_t kValueSize = 4;

  Config();
  void Setup();
  int FindEntry(const char* name);
  int FindEntry_P(const char* name);
  float ReadFloat(int i);
  float ReadFloat_P(const char* name);
  void WriteFloat(int i, double value);
  int CalcHash();
  void Update() override;
  bool HandleCommand(CommandBuffer& cb);

  bool initialized;
  Version version;
private:
  static const char* const entries_[];
};

#define DECLARE_CONFIG_VALUE(const_name, config_name) extern const char const_name[] PROGMEM;

CONFIG_VALUES(DECLARE_CONFIG_VALUE)

#endif
