import esphome.codegen as cg
import esphome.config_validation as cv

from esphome.components import i2c, sensor
from esphome.const import (
  CONF_ID,
  CONF_BATTERY_VOLTAGE,
  UNIT_VOLT,
  DEVICE_CLASS_VOLTAGE,
  STATE_CLASS_MEASUREMENT,  
)

CODEOWNERS = ["@iwtiwt"]
DEPENDENCIES = ["i2c"]

sy6970_ns = cg.esphome_ns.namespace("sy6970")
SY6970 = sy6970_ns.class_(
    "SY6970",
    cg.PollingComponent,
    i2c.I2CDevice,
)

CONF_STATE_LED_ENABLE = "state_led_enable"
CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(SY6970),
        cv.Optional(CONF_STATE_LED_ENABLE, default=True): cv.boolean,
        cv.Optional(CONF_BATTERY_VOLTAGE ): sensor.sensor_schema(
            unit_of_measurement=UNIT_VOLT,
            accuracy_decimals=2,
            device_class=DEVICE_CLASS_VOLTAGE,
            state_class=STATE_CLASS_MEASUREMENT,
            ),
    }
).extend(
    cv.polling_component_schema("60s")
).extend(
    i2c.i2c_device_schema(0x6A)
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await i2c.register_i2c_device(var, config)
    await cg.register_component(var, config)

    if enabled := config.get(CONF_STATE_LED_ENABLE):
        cg.add(var.set_state_led_enabled(enabled))
    if CONF_BATTERY_VOLTAGE in config:
        sens = await sensor.new_sensor(config[CONF_BATTERY_VOLTAGE])
        cg.add(var.set_batt_voltage_sensor(sens))