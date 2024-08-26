#include "esp32_ble_controller_espIdf.h"
#include "esphome/core/log.h"

namespace esphome {
namespace simple_led {

static const char *TAG = "simple_led";

void SimpleLEDComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up Simple LED Component...");
  this->pin_->setup();
  this->pin_->digital_write(false);  // LED'i başlangıçta kapalı yap
}

void SimpleLEDComponent::loop() {
  // Buraya istenirse LED kontrol kodları eklenebilir
}

void SimpleLEDComponent::turn_on() {
  this->led_state_ = true;
  this->pin_->digital_write(true);
  ESP_LOGD(TAG, "LED turned on");
}

void SimpleLEDComponent::turn_off() {
  this->led_state_ = false;
  this->pin_->digital_write(false);
  ESP_LOGD(TAG, "LED turned off");
}

}  // namespace simple_led
}  // namespace esphome
