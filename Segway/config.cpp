#include "config.h"

#include "command_buffer.h"

#define DEFINE_CONFIG_VALUE(const_name, config_name) const char const_name[] PROGMEM = config_name;

CONFIG_VALUES(DEFINE_CONFIG_VALUE)

#define ADD_CONFIG_VALUE_TO_ARRAY(const_name, config_name) const_name,

const char* const Config::entries_[] PROGMEM = {
  CONFIG_VALUES(ADD_CONFIG_VALUE_TO_ARRAY)
  NULL
};

Config::Config() {
  initialized = false;
  version = 0;
}

void Config::Setup() {
  if (!initialized && ((int)ReadFloat(0) == CalcHash())) {
    initialized = true;
    ++version;
  }
}

int Config::FindEntry(const char* name) {
  for (int i = 0; (char*)pgm_read_word(&(entries_[i])) != NULL; ++i) {
    if (strcmp_P(name, (char*)pgm_read_word(&(entries_[i]))) == 0)
      return i;
  }
  return -1;
}

int Config::FindEntry_P(const char* name) {
  for (int i = 0; (char*)pgm_read_word(&(entries_[i])) != NULL; ++i) {
    if (name == (char*)pgm_read_word(&(entries_[i])))
      return i;
  }
  return -1;
}

float Config::ReadFloat(int i) {
  if (i == -1)
    halt(200);
  float value;
  byte* ptr = (byte*)&value;
  for (int j = 0; j < sizeof(float); ++j)
    ptr[j] = EEPROM.read(i * sizeof(float) + j);
  return value;
}

float Config::ReadFloat_P(const char* name) {
  int i = FindEntry_P(name);
  return ReadFloat(i);
}

void Config::WriteFloat(int i, double value) {
  Serial.println(value);
  if (i == -1)
    halt(200);
  byte* ptr = (byte*)&value;
  for (int j = 0; j < sizeof(float); ++j)
    EEPROM.write(i *  sizeof(float) + j, ptr[j]);
}

int Config::CalcHash() {
  int hash = 0;
  for (int i = 0; (char*)pgm_read_word(&(entries_[i])) != NULL; ++i) {
    const char* name = (char*)pgm_read_word(&(entries_[i]));
    for (int j = 0; pgm_read_byte(&name[j]) != 0; ++j)
      hash += (int)pgm_read_byte(&name[j]);
  }
  return hash;
}

void Config::Update() {
}

bool Config::HandleCommand(CommandBuffer& cb) {
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
