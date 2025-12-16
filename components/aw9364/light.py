import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import light, output
from esphome.const import CONF_OUTPUT_ID, CONF_OUTPUT

aw9364_ns = cg.esphome_ns.namespace("aw9364")
AW9364 = aw9364_ns.class_("AW9364", light.LightOutput)

CONFIG_SCHEMA = light.BRIGHTNESS_ONLY_LIGHT_SCHEMA.extend(
    {
        cv.GenerateID(CONF_OUTPUT_ID): cv.declare_id(AW9364),
        cv.Required(CONF_PIN): cv.All(cv.only_on_esp32, cv.uint8_t),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_OUTPUT_ID])
    # Set up the GPIO pin
    pin = await cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    # Set up the light
    await light.setup_light(var, config)
    await cg.register_component(var, config)