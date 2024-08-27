from esphome import automation
import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome.components import gpio
from esphome.const import CONF_PIN

simple_led_ns = cg.esphome_ns.namespace('custom_ble_controller')
SimpleLEDComponent = simple_led_ns.class_('CustomBLEController', cg.Component)


BLEEnableAction = simple_led_ns.class_("BLEControllerSEND", automation.Action)


CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(SimpleLEDComponent),
    cv.Required(CONF_PIN): pins.gpio_output_pin_schema,
}).extend(cv.COMPONENT_SCHEMA)

def to_code(config):
    var = cg.new_Pvariable(config[cv.GenerateID()],)
    yield cg.register_component(var, config)
    pin = yield cg.gpio_pin_expression(config[CONF_PIN])
    cg.add(var.set_pin(pin))

def turn_on_action(var):
    return cg.call(var.turn_on())

@automation.register_action("custom_ble_controller.enable", BLEEnableAction, cv.Schema({}))
async def ble_enable_to_code(config, action_id, template_arg, args):
    return cg.new_Pvariable(action_id, template_arg)


