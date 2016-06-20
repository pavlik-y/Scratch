#ifndef EFFECT_CONTROLLER_H_
#define EFFECT_CONTROLLER_H_

class Adafruit_NeoPixel;
class BleCommand;

class EffectController {
 public:
//  EffectController(BleCommand* ble_command, Adafruit_NeoPixel* strip);
  void Tick();
  
 private:
//  BleCommand* ble_command_;
  Adafruit_NeoPixel* strip_;
};

#endif
