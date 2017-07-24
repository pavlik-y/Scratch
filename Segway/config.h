#ifndef CONFIG_H_
#define CONFIG_H_

#include <Arduino.h>
#include <EEPROM.h>

#include "command_buffer.h"
#include "common.h"
#include "component.h"

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

  Config() {
    initialized = false;
    version = 0;
  }

  void Setup() {
    if (!initialized && ((int)ReadFloat(0) == CalcHash())) {
      initialized = true;
      ++version;
    }
  }

  int FindEntry(const char* name) {
    for (int i = 0; (char*)pgm_read_word(&(entries_[i])) != NULL; ++i) {
      if (strcmp_P(name, (char*)pgm_read_word(&(entries_[i]))) == 0)
        return i;
    }
    return -1;
  }

  int FindEntry_P(const char* name) {
    for (int i = 0; (char*)pgm_read_word(&(entries_[i])) != NULL; ++i) {
      if (name == (char*)pgm_read_word(&(entries_[i])))
        return i;
    }
    return -1;
  }

  float ReadFloat(int i) {
    if (i == -1)
      halt(200);
    float value;
    byte* ptr = (byte*)&value;
    for (int j = 0; j < sizeof(float); ++j)
      ptr[j] = EEPROM.read(i * sizeof(float) + j);
    return value;
  }

  float ReadFloat_P(const char* name) {
    int i = FindEntry_P(name);
    return ReadFloat(i);
  }

  void WriteFloat(int i, double value) {
    Serial.println(value);
    if (i == -1)
      halt(200);
    byte* ptr = (byte*)&value;
    for (int j = 0; j < sizeof(float); ++j)
      EEPROM.write(i *  sizeof(float) + j, ptr[j]);
  }

  int CalcHash() {
    int hash = 0;
    for (int i = 0; (char*)pgm_read_word(&(entries_[i])) != NULL; ++i) {
      const char* name = (char*)pgm_read_word(&(entries_[i]));
      for (int j = 0; pgm_read_byte(&name[j]) != 0; ++j)
        hash += (int)pgm_read_byte(&name[j]);
    }
    return hash;
  }

  virtual void Update() {
  }

  virtual bool HandleCommand(CommandBuffer& cb) {
    if (strcmp_P(cb.command, PSTR("RdConfig")) == 0) {
      cb.BeginResponse();
      const char* name = cb.GetStringParam(0);
      int i = FindEntry(name);
      if (i == -1) {
        cb.WriteValue(name);
        cb.EndResponse();
        return true;
      }
      cb.WriteValue(i);
      cb.WriteValue(ReadFloat(i));
      cb.EndResponse();
      return true;
    }
    if (strcmp_P(cb.command, PSTR("WrConfig")) == 0) {
      cb.BeginResponse();
      const char* name = cb.GetStringParam(0);
      int i = FindEntry(name);
      if (i == -1) {
        cb.WriteValue(name);
        cb.EndResponse();
        return true;
      }
      WriteFloat(i, cb.GetFloatParam(1));
      Serial.println("done");
      cb.WriteValue("Ok");
      cb.EndResponse();
      if (!initialized && ((int)ReadFloat(0) == CalcHash())) {
        initialized = true;
      }
      ++version;
      return true;
    }
    if (strcmp_P(cb.command, PSTR("LstConfig")) == 0) {
      cb.BeginResponse();
      cb.WriteValue(initialized);
      cb.WriteValue(version);
      cb.WriteValue(CalcHash());
      char buf[64];
      for (int i = 0; (char*)pgm_read_word(&(entries_[i])) != NULL; ++i) {
        strcpy_P(buf, (char*)pgm_read_word(&(entries_[i])));
        cb.WriteValue(buf);
      }
      cb.EndResponse();
      return true;
    }
    return false;
  }

  bool initialized;
  Version version;
private:
  static const char* const entries_[];
};

#define DEFINE_CONFIG_VALUE(const_name, config_name) const char const_name[] PROGMEM = config_name;

CONFIG_VALUES(DEFINE_CONFIG_VALUE)

#define ADD_CONFIG_VALUE_TO_ARRAY(const_name, config_name) const_name,

const char* const Config::entries_[] PROGMEM = {
CONFIG_VALUES(ADD_CONFIG_VALUE_TO_ARRAY)
  NULL
};

#endif
