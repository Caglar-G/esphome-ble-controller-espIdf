import esphome.codegen as cg
from esphome.components import binary_sensor, esp32_ble_server, output, switch, gpio
import esphome.config_validation as cv
from esphome.const import CONF_ID

AUTO_LOAD = ["esp32_ble_server"]
CODEOWNERS = ["@jesserockz"]
DEPENDENCIES = ["wifi", "esp32"]

CONF_AUTHORIZED_DURATION = "authorized_duration"
CONF_AUTHORIZER = "authorizer"
CONF_BLE_SERVER_ID = "ble_server_id"
CONF_IDENTIFY_DURATION = "identify_duration"
CONF_STATUS_INDICATOR = "status_indicator"
CONF_WIFI_TIMEOUT = "wifi_timeout"

CONF_TEST = "test"


esp32_improv_ns = cg.esphome_ns.namespace("custom_ble_controller")
ESP32ImprovComponent = esp32_improv_ns.class_(
    "CustomBleController", cg.Component, esp32_ble_server.BLEServiceComponent
)



CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESP32ImprovComponent),
        cv.GenerateID(CONF_BLE_SERVER_ID): cv.use_id(esp32_ble_server.BLEServer),
        cv.Required(CONF_AUTHORIZER): cv.Any(
            cv.none, cv.use_id(binary_sensor.BinarySensor)
        ),
        cv.Optional(CONF_STATUS_INDICATOR): cv.use_id(output.BinaryOutput),
        cv.Optional(
            CONF_IDENTIFY_DURATION, default="10s"
        ): cv.positive_time_period_milliseconds,
        cv.Optional(
            CONF_AUTHORIZED_DURATION, default="1min"
        ): cv.positive_time_period_milliseconds,
        cv.Optional(
            CONF_WIFI_TIMEOUT, default="1min"
        ): cv.positive_time_period_milliseconds,
        cv.Optional("global_addr"): cv.use_id(globals),
        cv.Required(CONF_TEST): cv.use_id(switch.Switch),

    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    ble_server = await cg.get_variable(config[CONF_BLE_SERVER_ID])
    cg.add(ble_server.register_service_component(var))

    ga = await cg.get_variable(config["global_addr"])
    cg.add(var.set_global_addr(ga))

    test_switch = await cg.get_variable(config[CONF_TEST])
    cg.add(var.set_test(test_switch))

    cg.add(var.set_identify_duration(config[CONF_IDENTIFY_DURATION]))
    cg.add(var.set_authorized_duration(config[CONF_AUTHORIZED_DURATION]))

    cg.add(var.set_wifi_timeout(config[CONF_WIFI_TIMEOUT]))

    if CONF_AUTHORIZER in config and config[CONF_AUTHORIZER] is not None:
        activator = await cg.get_variable(config[CONF_AUTHORIZER])
        cg.add(var.set_authorizer(activator))

    if CONF_STATUS_INDICATOR in config:
        status_indicator = await cg.get_variable(config[CONF_STATUS_INDICATOR])
        cg.add(var.set_status_indicator(status_indicator))