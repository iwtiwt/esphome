#pragma once

#include "esphome/components/light/light_output.h"
#include "esphome/core/component.h"

namespace esphome {
namespace aw9364 {

class AW9364 : public light::LightOutput, public Component {
 public:
  AW9364(GPIOPin pin);
  void write_state(light::LightState *state) override;

 protected:
  GPIOPin pin_;
  uint8_t current_brightness_; 

};

}  // namespace aw9364
}  // namespace esphome