import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import gpio
from esphome.const import CONF_PIN

simple_led_ns = cg.esphome_ns.namespace('custom_ble_controller')
SimpleLEDComponent = simple_led_ns.class_('CustomBLEController', cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SimpleLEDComponent),
    cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()],)
    yield cg.register_component(var, config)
    pin = yield cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))
    turn_on_action = cg.ActionVariable('turn_on', var)
    turn_off_action = cg.ActionVariable('turn_off', var)
    cg.add(turn_on_action)
    cg.add(turn_off_action)

def turn_on_action(config, component):
    # This function is used to add the turn_on action to the YAML configuration
    cg.add(component.turn_on())
    
def turn_off_action(config, component):
    # This function is used to add the turn_off action to the YAML configuration
    cg.add(component.turn_off())

