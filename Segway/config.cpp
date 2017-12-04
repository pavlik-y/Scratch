#include "config.h"

#include "command_buffer.h"

namespace {
const int kValueSize = sizeof(float);
}

#define DEFINE_CONFIG_VALUE(const_name, config_name, default_value) \
  const char const_name[] PROGMEM = config_name;

CONFIG_VALUES(DEFINE_CONFIG_VALUE)

#define ADD_CONFIG_VALUE_TO_ARRAY(const_name, config_name, default_value) \
  const_name,

const char* const Config::entries_[] PROGMEM = {
  CONFIG_VALUES(ADD_CONFIG_VALUE_TO_ARRAY)
  NULL
};

ConfigStore::ConfigStore() = default;

void ConfigStore::Setup(const char* file_name, int values_count) {
  if (!config_file_.open(file_name, FS_ACCESS_READ | FS_ACCESS_WRITE))
    halt("Failed to open config file");
  values_count_ = values_count;
  if (config_file_.size() > values_count_ * kValueSize) {
    config_file_.close();
    if (!Nffs.remove(file_name))
      halt("nffs failed");
    if (!config_file_.open(file_name, FS_ACCESS_READ | FS_ACCESS_WRITE))
      halt("Failed to open config file");
  }
}

bool ConfigStore::IsValid() {
  return config_file_.size() == values_count_ * kValueSize;
}

float ConfigStore::ReadValue(int index) {
  if (!config_file_.seek(index * kValueSize))
    halt("Failed to seek config");
  float value;
  if (config_file_.read((uint8_t*)&value, kValueSize) != kValueSize) {
    Serial.println("Failed to read config");
    return -1.0;
  }
    // halt("Failed to read config");
  return value;
}

void ConfigStore::WriteValue(int index, float value) {
  if (!config_file_.seek(index * kValueSize))
    halt("Failed to seek config");
  if (config_file_.write((const uint8_t*)&value, kValueSize) != kValueSize)
    halt("Failed to write config");
}

int Config::GetConfigValuesCount() {
  return sizeof(entries_) / sizeof(entries_[0]) - 1;
}

Config::Config() = default;

void Config::Setup(ConfigStore* config_store) {
  config_store_ = config_store;
  if (!config_store_->IsValid() ||
      config_store_->ReadValue(0) != GetDefaultValue(0)) {
    ResetToDefaultValues();
  }
  // {PAV} Now config is always initialized, maybe |initialized| is redundant
  initialized = true;
  ++version;
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
    cb.WriteValue("Ok");
    cb.EndResponse();
    // if (!initialized && ((int)ReadFloat(0) == CalcHash())) {
    //   initialized = true;
    // }
    ++version;
    return true;
  }
  if (strcmp_P(cb.command, PSTR("LstConfig")) == 0) {
    cb.BeginResponse();
    cb.WriteValue(initialized);
    cb.WriteValue(version);
    char buf[64];
    for (int i = 0; i < GetConfigValuesCount(); ++i) {
      // const char* config_name = (const char*)pgm_read_ptr(&(entries_[i]));
      // strcpy_P(buf, config_name);
      const char* config_name = entries_[i];
      strcpy(buf, config_name);
      cb.WriteValue(buf);
    }
    cb.EndResponse();
    return true;
  }
  return false;
}

int Config::FindEntry(const char* name) {
  for (int i = 0; i < GetConfigValuesCount(); ++i) {
    // if (strcmp_P(name, (char*)pgm_read_word(&(entries_[i]))) == 0)
    if (strcmp(name, entries_[i]) == 0)
      return i;
  }
  return -1;
}

int Config::FindEntry_P(const char* name) {
  for (int i = 0; i < GetConfigValuesCount(); ++i) {
    // if (name == (char*)pgm_read_word(&(entries_[i])))
    if (name == entries_[i])
      return i;
  }
  return -1;
}

float Config::ReadFloat(int i) {
  return config_store_->ReadValue(i);
}

float Config::ReadFloat_P(const char* name) {
  int i = FindEntry_P(name);
  return ReadFloat(i);
}

void Config::WriteFloat(int i, double value) {
  config_store_->WriteValue(i, value);
}

void Config::ResetToDefaultValues() {
  for (int i = 0; i < GetConfigValuesCount(); i++)
    WriteFloat(i, GetDefaultValue(i));
}

#define RETURN_DEFAULT_VALUE(const_name, config_name, default_value) \
  if (value_iter == index) {        \
    return default_value;  \
  }                        \
  value_iter++;

float Config::GetDefaultValue(int index) {
  int value_iter = 0;
  CONFIG_VALUES(RETURN_DEFAULT_VALUE);
  return -1;
}
