#include "custom_ble_controller.h"


#include "esphome/components/esp32_ble/ble.h"
#include "esphome/components/esp32_ble_server/ble_2902.h"
#include "esphome/core/application.h"
#include "esphome/core/log.h"

  namespace esphome {
  namespace custom_ble_controller {

    static const char *TAG = "custom_ble_controller";

    void CustomBLEController::setup() {
      global_bleController = this;
      ESP_LOGCONFIG(TAG, "Setting up Simple LED Component...");
      this->pin_->setup();
      this->pin_->digital_write(false);  // LED'i başlangıçta kapalı yap
    }

    void CustomBLEController::loop() {
      if (!global_ble_server->is_running()) {
        /*
        this->state_ = improv::STATE_STOPPED;
        this->incoming_data_.clear();*/
        return;
      }
      // Buraya istenirse LED kontrol kodları eklenebilir
    }

    void CustomBLEController::turn_on() {
      this->led_state_ = true;
      this->pin_->digital_write(true);
      ESP_LOGD(TAG, "LED turned on");
    }

    void CustomBLEController::turn_off() {
      this->led_state_ = false;
      this->pin_->digital_write(false);
      ESP_LOGD(TAG, "LED turned off");
    }

    CustomBLEController *global_bleController = nullptr;

  }  // namespace simple_led
}  // namespace esphome
