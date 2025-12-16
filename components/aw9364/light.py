import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light
from esphome import pins
from esphome.const import CONF_OUTPUT_ID, CONF_PIN

aw9364_ns = cg.esphome_ns.namespace("aw9364")
AW9364 = aw9364_ns.class_("AW9364", light.LightOutput)

CONFIG_SCHEMA = light.BRIGHTNESS_ONLY_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(AW9364),
        cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    # Set up the GPIO pin
    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    await cg.register_component(var, config)