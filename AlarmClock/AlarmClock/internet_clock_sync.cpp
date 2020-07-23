#include "internet_clock_sync.h"

#include <Arduino.h>

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
#include <WiFiMulti.h>

constexpr int kSyncIntervalMs = 10000;

void InternetClockSync::Setup() {
  wifiMulti_.addAP("PY", "foo$bar00");
}

void InternetClockSync::Tick() {
  auto now = millis();
  if (now - last_sync_time_ >= kSyncIntervalMs) {
    last_sync_time_ = now;
    SyncTime();
  }
}

void InternetClockSync::SyncTime() {
  Serial.println("Time for syncing ");
  int status = wifiMulti_.run();
  if (status != WL_CONNECTED) {
    Serial.println("WiFi connection failed");
    Serial.println(status);
    return;
  }
  HTTPClient http;
  http.begin("http://worldtimeapi.org/api/ip");

  int httpCode = http.GET();

  if(httpCode <= 0) {
    Serial.println("HTTP failed");
    return;
  }
  if(httpCode != HTTP_CODE_OK) {
    Serial.println("HTTP code != OK");
    return;
  }
  String payload = http.getString();
  Serial.println(payload);
  http.end();
  DynamicJsonDocument doc(600);
  DeserializationError error = deserializeJson(doc, payload);
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.c_str());
    return;
  }

  // Extract values
  Serial.println(F("Response:"));
  Serial.println(doc["datetime"].as<char*>());
}