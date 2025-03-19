#pragma once

#include "esphome/components/i2c/i2c.h"
#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/hal.h"

namespace esphome {
namespace sy6970 {

union ConfigurationRegister07 {
  uint8_t raw;
  struct {
    uint8_t jeita_current_sense : 1;
    uint8_t fast_charge_timer : 2;
    uint8_t charge_safety_timer : 1;
    uint8_t watchdog_timer : 2;
    uint8_t status_pin_enable : 1;
    uint8_t charge_termination : 1;
  } __attribute__((packed));
};

union ConfigurationRegister0E {
  uint8_t raw;
  struct {
    uint8_t batteryv : 7;
    uint8_t therm_state : 1;
  } __attribute__((packed));
};

union ConfigurationRegister0F {
  uint8_t raw;
  struct {
    uint8_t sysv : 7;
    uint8_t unused : 1;
  } __attribute__((packed));
};

union ConfigurationRegister11 {
  uint8_t raw;
  struct {
    uint8_t busv : 7;
    uint8_t busconnected : 1;
  } __attribute__((packed));
};
class SY6970 : public PollingComponent, public i2c::I2CDevice {
 public:
  void setup() override;
  void dump_config() override;
  void update() override;

  void set_state_led_enabled(bool enabled) {
    this->is_state_led_enabled_ = enabled;
  }

  void reset_default();

  void enable_state_led();
  void disable_state_led();
  void set_batt_voltage_sensor(sensor::Sensor *battery_voltage_sensor) { battery_voltage_sensor_ = battery_voltage_sensor; }
  void set_bus_voltage_sensor(sensor::Sensor *bus_voltage_sensor) { bus_voltage_sensor_ = bus_voltage_sensor; }
  void set_sys_voltage_sensor(sensor::Sensor *sys_voltage_sensor) { sys_voltage_sensor_ = sys_voltage_sensor; }
  void disable_watchdog();

 protected:
  i2c::ErrorCode get_register_bit(uint8_t reg, uint8_t bit, bool &out);
  i2c::ErrorCode set_register_bit(uint8_t reg, uint8_t bit);
  i2c::ErrorCode clear_register_bit(uint8_t reg, uint8_t bit);

 protected:
  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *bus_voltage_sensor_{nullptr};
  sensor::Sensor *sys_voltage_sensor_{nullptr};
  bool is_state_led_enabled_ = false;
};

}  // namespace sy6970
}  // namespace esphome
