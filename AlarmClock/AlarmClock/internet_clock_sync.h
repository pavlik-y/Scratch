#ifndef INTERNET_CLOCK_SYNC_H_
#define INTERNET_CLOCK_SYNC_H_

#include <WiFiMulti.h>
#include "component.h"

class InternetClockSync : public Component {
 public:
  void Setup() override;
  void Tick() override;

 private:
  void SyncTime();

  WiFiMulti wifiMulti_;
  unsigned long last_sync_time_ = 0;
};

#endif  // INTERNET_CLOCK_SYNC_H_