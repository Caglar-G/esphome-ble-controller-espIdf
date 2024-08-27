#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"
#include "esphome/core/defines.h"
#include "esphome/core/helpers.h"
#include "esphome/core/preferences.h"


#include "esphome/components/esp32_ble_server/ble_characteristic.h"
#include "esphome/components/esp32_ble_server/ble_server.h"

namespace esphome {
namespace custom_ble_controller {

class CustomBLEController : public Component,  public BLEServiceComponent {
 public:
  void setup() override;
  void loop() override;

  void set_pin(GPIOPin *pin) { this->pin_ = pin; }
  void turn_on();
  void turn_off();

 protected:
  GPIOPin *pin_;
  bool led_state_{false};

  BLEService *service_ = nullptr;
  BLECharacteristic *status_;
  BLECharacteristic *error_;
  BLECharacteristic *rpc_;
  BLECharacteristic *rpc_response_;
  BLECharacteristic *capabilities_;

};

extern CustomBLEController *global_bleController;

template<typename... Ts> class BLEControllerSEND : public Action<Ts...> {
 public:
  void play(Ts... x) override { global_bleController->turn_on(); }
};


}  // namespace simple_led
}  // namespace esphome
