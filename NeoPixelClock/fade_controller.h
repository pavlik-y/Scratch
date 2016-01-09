#ifndef fade_controller_h_
#define fade_controller_h_

#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

void halt() {
 while (true) {
   digitalWrite(13, HIGH);
   delay(200);
   digitalWrite(13, LOW);
   delay(200);
 }
}

class FadeController {
public:
  
  static const int kHandCount = 3;
  static const int kPixelEntryCount = 6;
  static const uint32_t kFadeDuration = 900;
  
  struct TransitionEntry {
    bool active;
    uint32_t start_time;
    int from_pixel;
    int to_pixel;
  };
  
  struct PixelEntry {
    int pixel;
    uint32_t color;
  };
  
  void Setup(int pixel_pin, int dial_offset) {
    pixels_ = new Adafruit_NeoPixel(60, pixel_pin, NEO_GRB + NEO_KHZ800);
    pixels_->begin();
    for (int i = 0; i < kHandCount; ++i) {
      entries_[i].active = false;
      entries_[i].to_pixel = -1;
    }
    pixel_entries_size_ = 0;
    dial_offset_ = dial_offset;
  }
  
  void Clear() {
    pixels_->clear();
  }
  
  PixelEntry* FindPixelEntry(int pixel) {
    for (int i = 0; i < pixel_entries_size_; i++) {
      if (pixel_entries_[i].pixel == pixel) 
        return &pixel_entries_[i];
    }
    if (pixel_entries_size_ < kPixelEntryCount) {
      PixelEntry* result = &pixel_entries_[pixel_entries_size_];
      ++pixel_entries_size_;
      result->pixel = pixel;
      result->color = 0;
      return result;
    }
    halt();
  }
  
  void StartTransition(int hand, int from_pixel, int to_pixel) {
    from_pixel = (from_pixel + dial_offset_) % 60;
    to_pixel = (to_pixel + dial_offset_) % 60;
    TransitionEntry* entry = &entries_[hand];
    entry->active = true;
    entry->start_time = millis();
    entry->from_pixel = from_pixel;
    entry->to_pixel = to_pixel;
    Serial.println("StartTransition");
  }
  
  void SetBrightness(int brightness) {
    pixels_->setBrightness(brightness);
  }
  
  void Update() {
    pixels_->clear();
    uint32_t now = millis();
    bool updated = false;
    for (int i = 0; i < kHandCount; ++i) {
      TransitionEntry* entry = entries_ + i;
      if (now - entry->start_time > kFadeDuration)
        entry->active = false;
      if (!entry->active) {
        PixelEntry* pixel_entry = FindPixelEntry(entry->to_pixel);
        pixel_entry->color |= uint32_t(0xff) << (i * 8);
        continue;
      }
      uint32_t value = ((now - entry->start_time) * 255) / kFadeDuration;
      PixelEntry* fade_out_pixel_entry = FindPixelEntry(entry->from_pixel);
      fade_out_pixel_entry->color |= (0xff - value) << (i * 8);
      PixelEntry* fade_in_pixel_entry = FindPixelEntry(entry->to_pixel);
      fade_in_pixel_entry->color |= value << (i * 8);
    }
    for (int i = 0; i < pixel_entries_size_; i++) {
      pixels_->setPixelColor(pixel_entries_[i].pixel, pixel_entries_[i].color);
    }
    pixel_entries_size_ = 0;
    pixels_->show();
  }
  
public:
  Adafruit_NeoPixel* pixels_;

  TransitionEntry entries_[kHandCount];
  PixelEntry pixel_entries_[kPixelEntryCount];
  int pixel_entries_size_;
  int dial_offset_;
};

#endif
