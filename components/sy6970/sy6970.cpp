#include "sy6970.h"
#include "pmu.h"

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace sy6970 {

#define _BV(b) (1ULL << (uint64_t)(b))
#define ERROR_CHECK(err) \
  if ((err) != i2c::ERROR_OK) { \
    this->status_set_warning("Failed to communicate"); \
    return; \
  }

#define ERROR_CHECK_RET(err) \
  if ((err) != i2c::ERROR_OK) { \
    this->status_set_warning("Failed to communicate"); \
    return err; \
  }

namespace {
  constexpr static const char *const TAG = "sy6970";
} // anonymous namespace

void SY6970::setup() {
  ESP_LOGCONFIG(TAG, "Setting up SY6970 PMU...");

  this->disable_watchdog();
  this->is_state_led_enabled_ ? this->enable_state_led() : this->disable_state_led();
  this->is_adc_rate_enabled_ ? this->enable_adc_rate() : this->disable_adc_rate();

  ESP_LOGCONFIG(TAG, "SY6970 PMU setup complete");
}

void SY6970::update() {
  i2c::ErrorCode err;
  if (this->battery_voltage_sensor_ != nullptr) {
    ConfigurationRegister0E battery_register;
    err =this->read_register(POWERS_PPM_REG_0EH, &battery_register.raw, 1);
    ERROR_CHECK(err);

    float battery_voltage_v = battery_register.batteryv;
    battery_voltage_v *= 0.02f;
    battery_voltage_v += 2.304f;
    this->battery_voltage_sensor_->publish_state(battery_voltage_v);
  }
  if (this->sys_voltage_sensor_ != nullptr) {
    ConfigurationRegister0F sys_register;
    err =this->read_register(POWERS_PPM_REG_0FH, &sys_register.raw, 1);
    ERROR_CHECK(err);

    float sys_voltage_v = sys_register.sysv;
    sys_voltage_v *= 0.02f;
    sys_voltage_v += 2.304f;
    this->sys_voltage_sensor_->publish_state(sys_voltage_v);
  }
  if (this->bus_voltage_sensor_ != nullptr) {
    ConfigurationRegister11 bus_register;
    err =this->read_register(POWERS_PPM_REG_11H, &bus_register.raw, 1);
    ERROR_CHECK(err);

    float bus_voltage_v = bus_register.busv;
    bus_voltage_v *= 0.100f;
    bus_voltage_v += 2.6f;
    this->bus_voltage_sensor_->publish_state(bus_voltage_v);
  }
  if (this->charge_current_sensor_ != nullptr) {
    ConfigurationRegister12 charge_register;
    err =this->read_register(POWERS_PPM_REG_12H, &charge_register.raw, 1);
    ERROR_CHECK(err);

    float charge_current = charge_register.chgi;
    charge_current *= 0.05f;
    this->charge_current_sensor_->publish_state(charge_current);
  }
}

void SY6970::dump_config() {
  ESP_LOGCONFIG(TAG, "SY6970 PMU:");
  LOG_I2C_DEVICE(this);
  ESP_LOGCONFIG(TAG, "  State LED: %s", ONOFF(this->is_state_led_enabled_));
  ESP_LOGCONFIG(TAG, "   ADC Rate: %s", ONOFF(this->is_adc_rate_enabled_));
}

void SY6970::reset_default() {
  i2c::ErrorCode err = this->set_register_bit(POWERS_PPM_REG_14H, 7);
  ERROR_CHECK(err);
}

void SY6970::enable_state_led() {
  ConfigurationRegister07 val;
  i2c::ErrorCode err = this->read_register(POWERS_PPM_REG_07H, &val.raw, 1);
  ERROR_CHECK(err);

  val.status_pin_enable = 0;
  
  err =  this->write_register(POWERS_PPM_REG_07H, &val.raw, 1);
  ERROR_CHECK(err);
}

void SY6970::disable_state_led() {
  ConfigurationRegister07 val;
  i2c::ErrorCode err = this->read_register(POWERS_PPM_REG_07H, &val.raw, 1);
  ERROR_CHECK(err);

  val.status_pin_enable = 1;
  
  err = this->write_register(POWERS_PPM_REG_07H, &val.raw, 1);
  ERROR_CHECK(err);
}

void SY6970::enable_adc_rate() {
  ConfigurationRegister02 val;
  i2c::ErrorCode err = this->read_register(POWERS_PPM_REG_02H, &val.raw, 1);
  ERROR_CHECK(err);

  val.conv_rate = 1;
  
  err =  this->write_register(POWERS_PPM_REG_02H, &val.raw, 1);
  ERROR_CHECK(err);
}

void SY6970::disable_adc_rate() {
  ConfigurationRegister02 val;
  i2c::ErrorCode err = this->read_register(POWERS_PPM_REG_02H, &val.raw, 1);
  ERROR_CHECK(err);

  val.conv_rate = 0;
  
  err = this->write_register(POWERS_PPM_REG_02H, &val.raw, 1);
  ERROR_CHECK(err);
}

void SY6970::disable_watchdog() {
ConfigurationRegister07 val;
  i2c::ErrorCode err = this->read_register(POWERS_PPM_REG_07H, &val.raw, 1);
  ERROR_CHECK(err);

  val.watchdog_timer = 0;
  
  err = this->write_register(POWERS_PPM_REG_07H, &val.raw, 1);
  ERROR_CHECK(err);
}

}  // namespace sy6970
}  // namespace esphome
