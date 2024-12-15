#include <Arduino.h>
#include "Matter.h"
#include <app/server/OnboardingCodesUtil.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <ir_Sharp.h>

using namespace chip;
using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::endpoint;

// BME280の設定
// ref: https://mirushirutechru.com/techru/3201/
const int BME_CS = 15;
const int BME_SCK = 14;
const int BME_MOSI = 13;
const int BME_MISO = 12;
Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK);

float lastTemp = 0;
float lastHumid = 0;

// IRremoteの設定
const uint16_t kIrLed = 4;
IRSharpAc ac(kIrLed);  // Set the GPIO to be used to sending the message.

// Cluster and attribute ID used by Matter Temperature censor device
const uint32_t CLUSTER_ID_TEMP = TemperatureMeasurement::Id;
const uint32_t ATTRIBUTE_ID_TEMP = TemperatureMeasurement::Attributes::MeasuredValue::Id;
// Endpoint and attribute ref that will be assigned to Matter device
uint16_t temp_endpoint_id = 0;

// Cluster and attribute ID used by Matter Humidity censor device
const uint32_t CLUSTER_ID_HUMID = RelativeHumidityMeasurement::Id;
const uint32_t ATTRIBUTE_ID_HUMID = RelativeHumidityMeasurement::Attributes::MeasuredValue::Id;
// Endpoint and attribute ref that will be assigned to Matter device
uint16_t humid_endpoint_id = 0;

// Cluster and attribute ID used by Matter thermostat device
const uint32_t CLUSTER_ID_THERM = Thermostat::Id;
// Endpoint and attribute ref that will be assigned to Matter device
uint16_t therm_endpoint_id = 0;
// attribute_t *attribute_ref;

esp_matter_attr_val_t matterValue;  //share variable that should be set locally prior to usage

// エアコン（Thermostat）の状態を保持する変数を用意
// ここでデフォルト値を入れておく
// モード
int systemMode = 0; // 0:Off, 1:Auto, 3:Cool, 4:Heat
// 冷房関係
int occupiedCoolingSetpoint = 2600;
const int maxCoolingSetpoint = 3200;
const int minCoolingSetpoint = 1800;
// 暖房関係
int occupiedHeatingSetpoint = 2400;
const int maxHeatingSetpoint = 3200;
const int minHeatingSetpoint = 1800;

/* Function prototypes Begin */
void printState();
void sendIr();
void sendIrDebounced();
void checkSendIr();
void print_endpoint_info(String clusterName, endpoint_t *endpoint);
/* Function prototypes End */

void printState() {
  // Display the settings.
  Serial.println("Sharp A/C remote is in the following state:");
  Serial.printf("  %s\n", ac.toString().c_str());
  // Display the encoded IR sequence.
  unsigned char* ir_code = ac.getRaw();
  Serial.print("IR Code: 0x");
  for (uint8_t i = 0; i < kSharpAcStateLength; i++)
    Serial.printf("%02X", ir_code[i]);
  Serial.println();
}

long lastSendTime = 0;
unsigned long debounceInterval = 1000;
bool irPending = false;
bool prevState = false;

void sendIrDebounced() {
  irPending = true;
  lastSendTime = millis();
}

void checkSendIr() {
  if (irPending && (millis() - lastSendTime >= debounceInterval)) {
    sendIr();
    irPending = false;
  }
}

// static void sendIr() {
void sendIr() {
  // Set parameters
  bool newState;
  switch (systemMode) {
    case 0: // オフ
      newState = false;
      break;
    case 3: // 冷房
      newState = true;
      ac.setMode(kSharpAcCool);
      // TODO: 小数点以下四捨五入する
      ac.setTemp(occupiedCoolingSetpoint / 100);
      break;
    case 4: // 暖房
      newState = true;
      ac.setMode(kSharpAcHeat);
      // TODO: 小数点以下四捨五入する
      ac.setTemp(occupiedHeatingSetpoint / 100);
      break;
    default:
      Serial.println("WARN: Unknown System Mode");
      break;
  }
  
  ac.setPower(newState, prevState);
  prevState = newState;

  Serial.println("Sending IR command to A/C ...");
  ac.send();

  Serial.print(" New State: ");
  Serial.print(newState);
  Serial.print(" Prev State: ");
  Serial.println(prevState);
  printState();
}

// There is possibility to listen for various device events, related for example to setup process
// Leaved as empty for simplicity
static void on_device_event(const ChipDeviceEvent *event, intptr_t arg) {}
static esp_err_t on_identification(identification::callback_type_t type, uint16_t endpoint_id,
                                   uint8_t effect_id, uint8_t effect_variant, void *priv_data) {
  return ESP_OK;
}

// Listener on attribute update requests.
int newValue = 0;
static esp_err_t on_attribute_update(attribute::callback_type_t type, uint16_t endpoint_id, uint32_t cluster_id,
                                     uint32_t attribute_id, esp_matter_attr_val_t *val, void *priv_data) {
  if (type == attribute::PRE_UPDATE) {
    Serial.print("Update on endpoint: ");
    Serial.print(endpoint_id);
    Serial.print(" cluster: ");
    Serial.print(cluster_id);
    Serial.print(" attribute: ");
    Serial.print(attribute_id);
    newValue = val->val.i;
    Serial.print(" value: ");
    Serial.println(newValue);

    if (endpoint_id == therm_endpoint_id && cluster_id == CLUSTER_ID_THERM) {
      // Update the thermostat occupied setpoint value
      if (attribute_id == Thermostat::Attributes::SystemMode::Id) {
        // モード変更
        Serial.print(" NEW THERM MODE: ");
        Serial.println(newValue);
        systemMode = newValue;
        sendIrDebounced();
      }
      if (attribute_id == Thermostat::Attributes::OccupiedCoolingSetpoint::Id) {
        Serial.print(" NEW COOLING TEMP: ");
        Serial.println(newValue);
        occupiedCoolingSetpoint = newValue;
        sendIrDebounced();
      }
      if (attribute_id == Thermostat::Attributes::OccupiedHeatingSetpoint::Id) {
        Serial.print(" NEW HEATING TEMP: ");
        Serial.println(newValue);
        occupiedHeatingSetpoint = newValue;
        sendIrDebounced();
      }
    }
  }
  return ESP_OK;
}

void print_endpoint_info(String clusterName, endpoint_t *endpoint) {
  uint16_t endpoint_id = endpoint::get_id(endpoint);
  Serial.print(clusterName);
  Serial.print(" has endpoint: ");
  Serial.println(endpoint_id);
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Enable debug logging
  esp_log_level_set("*", ESP_LOG_DEBUG);

  // ファブリックインデックス0のすべてのエントリを削除（必要なときだけコメントアウトを解除する）
  chip::Access::AccessControl accessControl;
  accessControl.DeleteAllEntriesForFabric(0x2);

  // Setup BME280
  bme.begin();
  lastTemp = bme.readTemperature();
  lastHumid = bme.readHumidity();

  // Setup IRremote
  ac.begin();
  ac.setModel(sharp_ac_remote_model_t::A903);
  ac.setFan(kSharpAcFanAuto);
  ac.setSwingV(0);
  ac.setTurbo(false);
  ac.setIon(false);
  ac.setClean(false);

  // Setup Matter node
  node::config_t node_config;
  snprintf(node_config.root_node.basic_information.node_label, sizeof(node_config.root_node.basic_information.node_label), "ESP32 Hub");
  node_t *node = node::create(&node_config, on_attribute_update, on_identification);

  bridged_node::config_t bridged_node_config;
  bridged_node_config.bridged_device_basic_information.cluster_revision = 1;
  bridged_node_config.bridged_device_basic_information.reachable = true;

  // ブリッジ(Aggregator)のエンドポイントを作成
  endpoint_t *aggr_endpoint = aggregator::create(node, ENDPOINT_FLAG_NONE, NULL);

  // Setup temperature censor
  endpoint_t *temp_endpoint;
  temperature_sensor::config_t temp_config;
  temp_config.temperature_measurement.measured_value = lastTemp * 100;

  temp_endpoint = bridged_node::create(node, &bridged_node_config, ENDPOINT_FLAG_NONE, NULL);
  temperature_sensor::add(temp_endpoint, &temp_config);
  print_endpoint_info("temperature_sensor", temp_endpoint);

  temp_endpoint_id = endpoint::get_id(temp_endpoint);
  print_endpoint_info("temperature_sensor_endpoint", temp_endpoint);

  // Setup humidity censor
  endpoint_t *humid_endpoint;
  humidity_sensor::config_t humid_config;
  humid_config.relative_humidity_measurement.measured_value = lastHumid * 100;

  humid_endpoint = bridged_node::create(node, &bridged_node_config, ENDPOINT_FLAG_NONE, NULL);
  humidity_sensor::add(humid_endpoint, &humid_config);
  print_endpoint_info("humidity_sensor", humid_endpoint);

  humid_endpoint_id = endpoint::get_id(humid_endpoint);
  print_endpoint_info("humidity_sensor_endpoint", humid_endpoint);

  // Setup thermostat
  endpoint_t *therm_endpoint;
  cluster_t *therm_cluster;
  thermostat::config_t therm_config;
  therm_config.thermostat.local_temperature = lastTemp * 100; // センサーから取得した値を入れる
  therm_config.thermostat.system_mode = 0; // デフォルトはオフにする

  therm_endpoint = bridged_node::create(node, &bridged_node_config, ENDPOINT_FLAG_NONE, NULL);
  thermostat::add(therm_endpoint, &therm_config);
  print_endpoint_info("thermostat", therm_endpoint);

  therm_endpoint_id = endpoint::get_id(therm_endpoint);
  print_endpoint_info("Thermostat_endpoint", therm_endpoint);

  // 各デバイスに名前をつける
  cluster::bridged_device_basic_information::attribute::create_node_label(cluster::get(temp_endpoint, BridgedDeviceBasicInformation::Id), "Temp Censor", strlen("Temp Censor"));
  cluster::bridged_device_basic_information::attribute::create_node_label(cluster::get(humid_endpoint, BridgedDeviceBasicInformation::Id), "Humid Censor", strlen("Humid Censor"));
  cluster::bridged_device_basic_information::attribute::create_node_label(cluster::get(therm_endpoint, BridgedDeviceBasicInformation::Id), "Thermostat", strlen("Thermostat"));

  // 各デバイスのエンドポイントをブリッジに紐付ける
  set_parent_endpoint(temp_endpoint, aggr_endpoint);
  set_parent_endpoint(humid_endpoint, aggr_endpoint);
  set_parent_endpoint(therm_endpoint, aggr_endpoint);

  // Add additional feature
  therm_cluster = cluster::get(therm_endpoint, CLUSTER_ID_THERM);
  cluster::thermostat::feature::cooling::config_t cooling_config;
  cluster::thermostat::feature::heating::config_t heating_config;

  // units of temperature are in 0.01 Celsius for matter interactions
  cooling_config.abs_max_cool_setpoint_limit = maxCoolingSetpoint;
  cooling_config.abs_min_cool_setpoint_limit = minCoolingSetpoint;
  cooling_config.max_cool_setpoint_limit = maxCoolingSetpoint;
  cooling_config.min_cool_setpoint_limit = minCoolingSetpoint;
  cooling_config.pi_cooling_demand = 0;
  cooling_config.occupied_cooling_setpoint = occupiedCoolingSetpoint;
  cluster::thermostat::feature::cooling::add(therm_cluster, &cooling_config);

  heating_config.abs_max_heat_setpoint_limit = maxHeatingSetpoint;
  heating_config.abs_min_heat_setpoint_limit = minHeatingSetpoint;
  heating_config.max_heat_setpoint_limit = maxHeatingSetpoint;
  heating_config.min_heat_setpoint_limit = minHeatingSetpoint;
  heating_config.pi_heating_demand = 0;
  heating_config.occupied_heating_setpoint = occupiedHeatingSetpoint;
  cluster::thermostat::feature::heating::add(therm_cluster, &heating_config);

  //set heating step size
  matterValue = esp_matter_int16(10);  //10C
  attribute::update(therm_endpoint_id, CLUSTER_ID_THERM, Thermostat::Attributes::SetpointChangeAmount::Id, &matterValue);

  // Setup DAC (this is good place to also set custom commission data, passcodes etc.)
  esp_matter::set_custom_dac_provider(chip::Credentials::Examples::GetExampleDACProvider());

  // Start Matter device
  esp_matter::start(on_device_event);

  // Print codes needed to setup Matter device
  PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));
}

unsigned long timer = 0;

void loop() {
  // 定期的にIR送信が必要ないかチェック
  checkSendIr();

  if (millis() > timer) {
    timer = millis() + 10000;  // update timer
    float newTemp = bme.readTemperature();
    float newHumid = bme.readHumidity();

    Serial.print(" TEMP: ");
    Serial.print(newTemp);
    Serial.print(" HUMID: ");
    Serial.println(newHumid);

    if (lastTemp != newTemp) {
      Serial.println("Temp has changed!");
      lastTemp = newTemp;

      matterValue = esp_matter_int16(newTemp * 100);
      attribute::update(therm_endpoint_id, CLUSTER_ID_THERM, Thermostat::Attributes::LocalTemperature::Id, &matterValue);
      attribute::update(temp_endpoint_id, CLUSTER_ID_TEMP, ATTRIBUTE_ID_TEMP, &matterValue);
    }
    if (lastHumid != newHumid) {
      Serial.println("Humid has changed!");
      lastHumid = newHumid;

      matterValue = esp_matter_int16(newHumid * 100);
      attribute::update(humid_endpoint_id, CLUSTER_ID_HUMID, ATTRIBUTE_ID_HUMID, &matterValue);
    }
    
  }
}