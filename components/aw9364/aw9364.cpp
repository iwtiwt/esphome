#include "aw9364.h"
#include "esphome/core/log.h"

namespace esphome {
namespace aw9364 {

static const char *TAG = "aw9364";

light::LightTraits AW9364::get_traits() {
  auto traits = light::LightTraits();
  traits.set_supported_color_modes({light::ColorMode::BRIGHTNESS});
  return traits;
}

void AW9364::control(const fan::FanCall &call) {
  if (call.get_state().has_value())
    this->state = *call.get_state();
  if (call.get_oscillating().has_value())
    this->oscillating = *call.get_oscillating();

  this->write_state_();
  this->publish_state();
}

void AW9364::write_state_() {
  this->output_->set_state(this->state);
  if (this->oscillating_ != nullptr)
    this->oscillating_->set_state(this->oscillating);
}

void AW9364::dump_config() { LOG_FAN("", "E", this); }

}  // namespace aw9364
}  // namespace esphome