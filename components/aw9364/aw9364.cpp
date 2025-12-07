#include "aw9364.h"
#include "esphome/core/log.h"

namespace esphome {
namespace aw9364 {

static const char *TAG = "aw9364";

fan::FanTraits AW9364::get_traits() {
  return fan::FanTraits(this->oscillating_ != nullptr, false, this->direction_ != nullptr, 0);
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
  if (this->direction_ != nullptr)
    this->direction_->set_state(this->direction == fan::FanDirection::REVERSE);
}

void AW9364::dump_config() { LOG_FAN("", "E", this); }

}  // namespace aw9364
}  // namespace esphome