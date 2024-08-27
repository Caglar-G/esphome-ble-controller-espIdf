#include "custom_ble_controller.h"

#include "esphome/components/esp32_ble/ble.h"
#include "esphome/components/esp32_ble_server/ble_2902.h"
#include "esphome/core/application.h"
#include "esphome/core/log.h"

#ifdef USE_ESP32

namespace esphome {
namespace esp32_improv {

static const char *const TAG = "esp32_improv.component";
static const char *const ESPHOME_MY_LINK = "https://my.home-assistant.io/redirect/config_flow_start?domain=esphome";

ESP32ImprovComponent::ESP32ImprovComponent() { global_improv_component = this; }

void ESP32ImprovComponent::setup() {

}

void ESP32ImprovComponent::setup_characteristics() {
 this->status_ = this->service_->create_characteristic(
      "ade15296-b658-4021-8c2a-94c1fb1d7d41", BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  BLEDescriptor *status_descriptor = new BLE2902();
  this->status_->add_descriptor(status_descriptor);
}

void ESP32ImprovComponent::loop() {
   if (this->service_ == nullptr) {
    // Setup the service
    ESP_LOGD(TAG, "Creating Improv service");
    global_ble_server->create_service(ESPBTUUID::from_raw("c2d76977-2c7f-4ff7-ba08-673b6248e184"), true);
    this->service_ = global_ble_server->get_service(ESPBTUUID::from_raw("c2d76977-2c7f-4ff7-ba08-673b6248e184"));
    this->setup_characteristics();
  }
}

void ESP32ImprovComponent::set_status_indicator_state_(bool state) {
#ifdef USE_OUTPUT
  if (this->status_indicator_ == nullptr)
    return;
  if (this->status_indicator_state_ == state)
    return;
  this->status_indicator_state_ = state;
  if (state) {
    this->status_indicator_->turn_on();
  } else {
    this->status_indicator_->turn_off();
  }
#endif
}

bool ESP32ImprovComponent::check_identify_() {
  uint32_t now = millis();

  bool identify = this->identify_start_ != 0 && now - this->identify_start_ <= this->identify_duration_;

  if (identify) {
    uint32_t time = now % 1000;
    this->set_status_indicator_state_(time < 600 && time % 200 < 100);
  }
  return identify;
}



void ESP32ImprovComponent::send_response_(std::vector<uint8_t> &response) {
  
}

void ESP32ImprovComponent::start() {
  /*
  if (this->should_start_ || this->state_ != improv::STATE_STOPPED)
    return;

  ESP_LOGD(TAG, "Setting Improv to start");
  this->should_start_ = true;*/
}

void ESP32ImprovComponent::stop() {
  /*
  this->should_start_ = false;
  this->set_timeout("end-service", 1000, [this] {
    if (this->state_ == improv::STATE_STOPPED || this->service_ == nullptr)
      return;
    this->service_->stop();
    this->set_state_(improv::STATE_STOPPED);
  });*/
}

float ESP32ImprovComponent::get_setup_priority() const { return setup_priority::AFTER_BLUETOOTH; }

void ESP32ImprovComponent::dump_config() {
  ESP_LOGCONFIG(TAG, "ESP32 Improv:");
#ifdef USE_BINARY_SENSOR
  LOG_BINARY_SENSOR("  ", "Authorizer", this->authorizer_);
#endif
#ifdef USE_OUTPUT
  ESP_LOGCONFIG(TAG, "  Status Indicator: '%s'", YESNO(this->status_indicator_ != nullptr));
#endif
}

void ESP32ImprovComponent::process_incoming_data_() {
  
}

void ESP32ImprovComponent::on_wifi_connect_timeout_() {
 
}

void ESP32ImprovComponent::on_client_disconnect() {  };

ESP32ImprovComponent *global_improv_component = nullptr;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace esp32_improv
}  // namespace esphome

#endif