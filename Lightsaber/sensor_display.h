#ifndef SENSOR_DISPLAY_H_
#define SENSOR_DISPLAY_H_

#include <Adafruit_LSM9DS0.h>
#include <Adafruit_NeoPixel.h>

#include "component.h"

class SensorDisplay : public Component {
 public:
  enum SensorType {
    COMPASS,
    ACCELEROMETER,
    GYROSCOPE
  };
  
  SensorDisplay(Adafruit_NeoPixel* strip, Adafruit_LSM9DS0* sensor);

  void Start() override;
  void Stop() override;
  void Tick() override;
  void SetSensorType(SensorType sensor_type);
  
 private:
  void ReadCompass(int* x, int* y, int* z);
  void ReadAccelerometer(int* x, int* y, int* z);
  void ReadGyroscope(int* x, int* y, int* z);
  
  Adafruit_NeoPixel* strip_;
  Adafruit_LSM9DS0* sensor_;
  unsigned long last_sample_time_;
  SensorType sensor_type_;
};

#endif // SENSOR_DISPLAY_H_
