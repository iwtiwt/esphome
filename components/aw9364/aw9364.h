#pragma once

#include "esphome/components/fan/fan.h"
#include "esphome/components/output/binary_output.h"
#include "esphome/core/component.h"

namespace esphome {
namespace aw9364 {

class AW9364 : public Component, public fan::Fan {
 public:
  void dump_config() override;
  void set_output(output::BinaryOutput *output) { this->output_ = output; }
  void set_oscillating(output::BinaryOutput *oscillating) { this->oscillating_ = oscillating; }
  fan::FanTraits get_traits() override;

 protected:
  void control(const fan::FanCall &call) override;
  void write_state_();

  output::BinaryOutput *output_;
  output::BinaryOutput *oscillating_{nullptr};
};

}  // namespace empty_fan
}  // namespace esphome