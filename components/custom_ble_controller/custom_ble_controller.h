#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace custom_ble_controller {

class CustomBLEController : public Component {
 public:
  void setup() override;
  void loop() override;

  void set_pin(GPIOPin *pin) { this->pin_ = pin; }
  void turn_on();
  void turn_off();

 protected:
  GPIOPin *pin_;
  bool led_state_{false};
};

extern CustomBLEController *global_bleController;

template<typename... Ts> class BLEControllerSEND : public Action<Ts...> {
 public:
  void play(Ts... x) override { global_bleController->turn_on(); }
};


}  // namespace simple_led
}  // namespace esphome
