#include "aw9364.h"
#include "esphome/core/log.h"

namespace esphome {
namespace aw9364 {

static const char *TAG = "aw9364";


void AW9364::write_state(light::LightState *state) override {
  static uint8_t steps = 16;
  float brightness = state->current_values.get_brightness();
  uint8_t new_brightness = static_cast<uint8_t>(brightness * 16);
  if (new_brightness == 0) {
      pin_->digital_write(false);
      delay(3);
      current_brightness_ = 0;
      return;
  }
  if (current_brightness_ == 0) {
      pin_->digital_write(true);
      new_brightness = steps;
      delayMicroseconds(30);
  }
  int from = steps - current_brightness_;
  int to = steps - new_brightness;
  int num = (steps + to - from) % steps;
  for (int i = 0; i < num; i++) {
      pin_->digital_write(false);
      pin_->digital_write(true);
  }
  current_brightness_ = new_brightness;
}


}  // namespace aw9364
}  // namespace esphome