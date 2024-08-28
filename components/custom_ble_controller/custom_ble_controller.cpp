#include "custom_ble_controller.h"

#include "esphome/components/esp32_ble/ble.h"
#include "esphome/components/esp32_ble_server/ble_2902.h"
#include "esphome/core/application.h"
#include "esphome/core/log.h"
#include "esphome/components/globals/globals_component.h"
#include "esphome/core/helpers.h"

#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#include "esphome/components/mqtt/mqtt_client.h"


#include "esphome.h"

// Include global defines
#include "esphome/core/defines.h"

#ifdef USE_ESP32

namespace esphome {
namespace custom_ble_controller {

static const uint16_t DEVICE_INFORMATION_SERVICE_UUID = 0x180B;
static const uint16_t MODEL_UUID = 0x2A25;

static const char *const TAG = "esp32_improv.component";
static const char *const ESPHOME_MY_LINK = "https://my.home-assistant.io/redirect/config_flow_start?domain=esphome";

CustomBleController::CustomBleController() { global_improv_component = this; }

void CustomBleController::setup() {


  ESP_LOGD(TAG, "DeviceId: %s", id(global_forced_addr).c_str());
  mqtt::global_mqtt_client->set_client_id(id(global_forced_addr));

}

void CustomBleController::setup_characteristics() {
  this->status_ = this->service_->create_characteristic(
      ESPBTUUID::from_raw("87654321-4321-6789-4321-fedcba987654"), BLECharacteristic::PROPERTY_WRITE);

  this->status_->on_write([this](const std::vector<uint8_t> &data) {
    if (!data.empty()) {
      std::string data_str;
      for (size_t i = 0; i < data.size(); ++i) {
          data_str += data[i];
      }
      /*
      for (size_t i = 0; i < data.size(); ++i) {
          char buf[4];
          sprintf(buf, "%02X", data[i]);  // Her bir elemanı hexadecimal formatta yaz
          data_str += buf;
          if (i < data.size() - 1) {
              data_str += " ";  // Elemanlar arasına boşluk ekle
          }
      }*/
 
      ESP_LOGD(TAG, "Data: %s", data_str.c_str());
      mqtt::global_mqtt_client->set_client_id(data_str);
      id(global_forced_addr) = data_str;
      //this->incoming_data_.insert(this->incoming_data_.end(), data.begin(), data.end());
      //ESP_LOGD(TAG, "Creating Improv service");
    }
  });

  BLEDescriptor *status_descriptor = new BLE2902();
  this->status_->add_descriptor(status_descriptor);
  ESP_LOGD(TAG, "Improv service setup_characteristics");
  ESP_LOGD(TAG, "DeviceId: %s", id(global_forced_addr).c_str());

  
}

void CustomBleController::setDeviceId(globals::GlobalsComponent<std::string>  *deviceId){
  deviceId_ = deviceId;
}

void CustomBleController::loop() {
  if (mqtt::global_mqtt_client->is_running() && setupp == false) {
    mqtt::MQTTSwitchComponent* tetetet = new mqtt::MQTTSwitchComponent(this->test_switch_); 
    tetetet->set_custom_command_topic("adf");
    mqtt::global_mqtt_client->register_mqtt_component(tetetet);
    ESP_LOGD("example", "kaydedilddi");
    this->setupp = true;
  }
  if (!global_ble_server->is_running()) {
    this->incoming_data_.clear();
    return;
  }
  if (this->service_ == nullptr) {
    // Setup the service
    ESP_LOGD(TAG, "Creating Improv service");
    global_ble_server->create_service(ESPBTUUID::from_raw("12345678-1234-5678-1234-56789abcdef0"));
    this->service_ = global_ble_server->get_service(ESPBTUUID::from_raw("12345678-1234-5678-1234-56789abcdef0"));
    this->setup_characteristics();

    //esphome::globals::id(deviceId) = "tset";
    //deviceId->value() = "something";
    /*
    this->service_->start();
    esp32_ble::global_ble->advertising_start();*/
  }
  else{
    if (this->service_->is_created()) {
      //ESP_LOGD(TAG, "Service is_created!");
      if (this->service_->is_running()) {
          //ESP_LOGD(TAG, "Service is_running!");
          /*
          esp32_ble::global_ble->advertising_start();

          this->set_state_(improv::STATE_AWAITING_AUTHORIZATION);
          this->set_error_(improv::ERROR_NONE);
          ESP_LOGD(TAG, "Service started!");*/
        } else {
          this->service_->start();
        }
      }
  }

}

void CustomBleController::set_status_indicator_state_(bool state) {
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

bool CustomBleController::check_identify_() {
  uint32_t now = millis();

  bool identify = this->identify_start_ != 0 && now - this->identify_start_ <= this->identify_duration_;

  if (identify) {
    uint32_t time = now % 1000;
    this->set_status_indicator_state_(time < 600 && time % 200 < 100);
  }
  return identify;
}



void CustomBleController::send_response_(std::vector<uint8_t> &response) {
  
}

void CustomBleController::start() {
  ESP_LOGD(TAG, "Setting Improv to start First");
  if (this->should_start_)
    return;

  ESP_LOGD(TAG, "Setting Improv to start");
  this->should_start_ = true;
}

void CustomBleController::stop() {
  /*
  this->should_start_ = false;
  this->set_timeout("end-service", 1000, [this] {
    if (this->state_ == improv::STATE_STOPPED || this->service_ == nullptr)
      return;
    this->service_->stop();
    this->set_state_(improv::STATE_STOPPED);
  });*/
}

float CustomBleController::get_setup_priority() const { return setup_priority::AFTER_BLUETOOTH; }

void CustomBleController::dump_config() {
  ESP_LOGCONFIG(TAG, "ESP32 Improv:");
#ifdef USE_BINARY_SENSOR
  LOG_BINARY_SENSOR("  ", "Authorizer", this->authorizer_);
#endif
#ifdef USE_OUTPUT
  ESP_LOGCONFIG(TAG, "  Status Indicator: '%s'", YESNO(this->status_indicator_ != nullptr));
#endif
}

void CustomBleController::process_incoming_data_() {
  
}

void CustomBleController::on_wifi_connect_timeout_() {
 
}

void CustomBleController::on_client_disconnect() {  };

CustomBleController *global_improv_component = nullptr;  // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

}  // namespace esp32_improv
}  // namespace esphome

#endif