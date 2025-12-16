#include "aw9364.h"
#include "esphome/core/log.h"

namespace esphome {
namespace aw9364 {

static const char *TAG = "aw9364";


void AW9364::write_state(light::LightState *state) override {
  float brightness = state->current_values.get_brightness();
  uint8_t new_brightness = static_cast<uint8_t>(brightness * 16);
  setBrightness(new_brightness);
}

void AW9364::setBrightness(uint8_t value)
{
    static uint8_t steps = 16;
    if (value == 0) {
        pin_->digital_write(false);
        delay(3);
        current_brightness_ = 0;
        return;
    }
    if (current_brightness_ == 0) {
        pin_->digital_write(true);
        level = steps;
        delayMicroseconds(30);
    }
    int from = steps - current_brightness_;
    int to = steps - value;
    int num = (steps + to - from) % steps;
    for (int i = 0; i < num; i++) {
        pin_->digital_write(false);
        pin_->digital_write(true);
    }
    current_brightness_ = value;
}


}  // namespace aw9364
}  // namespace esphome