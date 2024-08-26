import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import gpio
from esphome.const import CONF_PIN

simple_led_ns = cg.esphome_ns.namespace('simple_led')
SimpleLEDComponent = simple_led_ns.class_('SimpleLEDComponent', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SimpleLEDComponent),
    cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()],)
    yield cg.register_component(var, config)
    pin = yield cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    
