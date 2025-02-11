#ifndef MATTER_FUNC_HPP_
#define MATTER_FUNC_HPP_

#define ENABLE_SENSOR_1 1
#define ENABLE_SENSOR_2 1
#define ENABLE_IR 1
#define ENABLE_IR_LIGHT 0
#define ENABLE_LIGHT_SWITCH 0

#include <Arduino.h>
#include "Matter.h"
#include <app/server/OnboardingCodesUtil.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include "matter_on_update.hpp"
#include "ClimateSensor.hpp"
#include "IrSendLight.hpp"
#include "IrSendAC.hpp"
#include "SpiDataSync.hpp"
using namespace chip;
namespace clusters = chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::endpoint;

#define CHIP_LOG_LEVEL CHIP_LOG_LEVEL_ERROR // エラーレベルのログのみ出力

// Matterプラグインユニットデバイスで使用されるクラスターと属性ID
// Matterデバイスに割り当てられるエンドポイントと属性参照
#if ENABLE_SENSOR_1
const uint32_t CLUSTER_ID_TEMP = clusters::TemperatureMeasurement::Id;
const uint32_t CLUSTER_ID_HUMID = clusters::RelativeHumidityMeasurement::Id;
const uint32_t CLUSTER_ID_PRESS = clusters::PressureMeasurement::Id;
const uint32_t ATTRIBUTE_ID_TEMP = clusters::TemperatureMeasurement::Attributes::MeasuredValue::Id;
const uint32_t ATTRIBUTE_ID_HUMID = clusters::RelativeHumidityMeasurement::Attributes::MeasuredValue::Id;
const uint32_t ATTRIBUTE_ID_PRESS = clusters::PressureMeasurement::Attributes::MeasuredValue::Id;
uint16_t temperature_sensor_endpoint_id = 0; // 温度センサ
uint16_t humidity_sensor_endpoint_id = 0; // 湿度センサ
uint16_t pressure_sensor_endpoint_id = 0; // 気圧センサ
attribute_t *attribute_ref_temperature;
attribute_t *attribute_ref_humidity;
attribute_t *attribute_ref_pressure;
#endif
#if ENABLE_SENSOR_2
const uint32_t CLUSTER_ID_LIGHT = clusters::IlluminanceMeasurement::Id;
const uint32_t CLUSTER_ID_OCCUP = clusters::OccupancySensing::Id;
const uint32_t ATTRIBUTE_ID_LIGHT = clusters::IlluminanceMeasurement::Attributes::MeasuredValue::Id;
const uint32_t ATTRIBUTE_ID_OCCUP = clusters::OccupancySensing::Attributes::Occupancy::Id;
uint16_t illuminance_sensor_endpoint_id = 0; // 照度センサ
uint16_t occupancy_sensor_endpoint_id = 0; // 占有センサ(人感センサ)
attribute_t *attribute_ref_illuminance;
attribute_t *attribute_ref_occupancy;
#endif
#if ENABLE_IR
const uint32_t CLUSTER_ID_AC = clusters::Thermostat::Id;
const uint32_t CLUSTER_ID_ONOFF = clusters::OnOff::Id;
const uint32_t CLUSTER_ID_FAN = clusters::FanControl::Id;
uint16_t ac_endpoint_id = 0; // エアコン
uint16_t fan_endpoint_id = 0; // 扇風機
attribute_t *attribute_ref_ac;
attribute_t *attribute_ref_fan;
// エアコン（Thermostat）の状態を保持する変数を用意
// ここでデフォルト値を入れておく
// モード
int systemMode = 0; // 0:Off, 1:Auto, 3:Cool, 4:Heat
// 冷房関係
int occupiedCoolingSetpoint = 2900;
const int maxCoolingSetpoint = 3200;
const int minCoolingSetpoint = 1800;
// 暖房関係
int occupiedHeatingSetpoint = 1600;
const int maxHeatingSetpoint = 3200;
const int minHeatingSetpoint = 1600;
esp_matter_attr_val_t matterValue;  //share variable that should be set locally prior to usage
#if ENABLE_IR_LIGHT || ENABLE_LIGHT_SWITCH
const uint32_t CLUSTER_ID_LEVEL = clusters::LevelControl::Id;
const uint32_t CLUSTER_ID_COLOR = clusters::ColorControl::Id;
uint16_t light_endpoint_id = 0; // 照明
attribute_t *attribute_ref_light;
#endif
#endif


// セットアッププロセスに関連するさまざまなデバイスイベントをリッスンする可能性があります。簡単のために空のままにしてあります。
static void on_device_event(const ChipDeviceEvent *event, intptr_t arg) {}
static esp_err_t on_identification(identification::callback_type_t type,
                   uint16_t endpoint_id, uint8_t effect_id,
                   uint8_t effect_variant, void *priv_data) {
  return ESP_OK;
}


/**
 * @brief 属性更新リクエストのリスナー
 * この例では、更新がリクエストされたとき、パス（エンドポイント、クラスター、属性）がプラグインユニット属性と一致するかどうかを確認します。もし一致する場合、LEDは新しい状態に変更されます。
 * 
 * @param type 属性更新のタイプ
 * @param endpoint_id エンドポイントID
 * @param cluster_id クラスターID
 * @param attribute_id 属性ID
 * @param val 属性値
 * @param priv_data プライベートデータ
 * @return esp_err_t 
 */
static esp_err_t on_attribute_update(attribute::callback_type_t type,
                   uint16_t endpoint_id, uint32_t cluster_id,
                   uint32_t attribute_id,
                   esp_matter_attr_val_t *val,
                   void *priv_data) {
    if (type == attribute::PRE_UPDATE) {
        #if ENABLE_IR
        Serial.printf("Attribute Update: endpoint_id=%d, cluster_id=%d, attribute_id=%d\n", endpoint_id, cluster_id, attribute_id);
        if (endpoint_id == ac_endpoint_id && cluster_id == CLUSTER_ID_AC) {
            if (attribute_id == clusters::Thermostat::Attributes::SystemMode::Id) {
                systemMode = val->val.u8;
                on_ac_system_mode_update((ac::SystemMode)systemMode);
                Serial.printf("SystemMode: %d\n", systemMode);
            } else if (attribute_id == clusters::Thermostat::Attributes::OccupiedCoolingSetpoint::Id) {
                occupiedCoolingSetpoint = val->val.i16;
                Serial.printf("OccupiedCoolingSetpoint: %d\n", occupiedCoolingSetpoint);
            } else if (attribute_id == clusters::Thermostat::Attributes::OccupiedHeatingSetpoint::Id) {
                occupiedHeatingSetpoint = val->val.i16;
                Serial.printf("OccupiedHeatingSetpoint: %d\n", occupiedHeatingSetpoint);
            } 
        #if ENABLE_IR_LIGHT
        } else if (endpoint_id == light_endpoint_id) {
            if (cluster_id == CLUSTER_ID_ONOFF && attribute_id == clusters::OnOff::Attributes::OnOff::Id) {
                uint8_t new_state = val->val.u8;
                on_light_onoff_update((light::OnOff)new_state);
                Serial.printf("Light: %d\n", new_state);
            } else if (cluster_id == CLUSTER_ID_LEVEL && attribute_id == clusters::LevelControl::Attributes::CurrentLevel::Id) {
                uint8_t new_level = val->val.u8;
                Serial.printf("Light Level: %d\n", new_level);
            } else if (cluster_id == CLUSTER_ID_COLOR && attribute_id == clusters::ColorControl::Attributes::ColorTemperatureMireds::Id) {
                uint16_t new_color = val->val.u16;
                Serial.printf("Light Color: %d\n", new_color);
            } else {
                Serial.printf("Unknown attribute: %d\n", attribute_id);
            }
        #endif
        } else if (endpoint_id == fan_endpoint_id && cluster_id == CLUSTER_ID_FAN) {
            if (attribute_id == clusters::FanControl::Attributes::FanMode::Id) {
                uint8_t new_state = val->val.u8; // 0:Off, 1:Low, 2:Medium, 3:High, 4:On
                Serial.printf("Fan: %d\n", new_state);
            } else if (attribute_id == clusters::FanControl::Attributes::FanModeSequence::Id) {
                uint8_t new_state = val->val.u8;
                Serial.printf("Fan Mode Sequence: %d\n", new_state);
            } else if (attribute_id == clusters::FanControl::Attributes::PercentSetting::Id) {
                uint8_t new_state = val->val.u8;
                Serial.printf("Fan Percent Setting: %d\n", new_state);
            } else if (attribute_id == clusters::FanControl::Attributes::PercentCurrent::Id) {
                uint8_t new_state = val->val.u8;
                Serial.printf("Fan Percent Current: %d\n", new_state);
            } else {
                Serial.printf("Unknown attribute: %d\n", attribute_id);
            }
        }
        #endif
    }
    return ESP_OK;
}

/**
 * @brief Matterデバイスのセットアップ
 * 
 */
void setup_matter() {
    Serial.println("Start to setup Matter device");

    // デバッグログを有効にする
    Serial.println("Enabling debug logs...");
    esp_log_level_set("*", ESP_LOG_DEBUG);

    // Matterノードをセットアップ
    Serial.println("Setting up Matter node...");
    node::config_t node_config;
    node_t *node =
    node::create(&node_config, on_attribute_update, on_identification);

    // on_off_plugin_unit::config_t plugin_unit_config;
    // plugin_unit_config.on_off.on_off = false;
    // plugin_unit_config.on_off.lighting.start_up_on_off = false;

    #if ENABLE_SENSOR_1
    // デフォルト値でプラグインユニットエンドポイント/クラスター/属性をセットアップ
    Serial.println("Setting up plugin unit endpoints...");
    temperature_sensor::config_t temperature_sensor_config;
    temperature_sensor_config.temperature_measurement.measured_value = 2500;
    humidity_sensor::config_t humidity_sensor_config;
    humidity_sensor_config.relative_humidity_measurement.measured_value = 5000;
    pressure_sensor::config_t pressure_sensor_config;
    pressure_sensor_config.pressure_measurement.pressure_measured_value = 1013;
    //　エンドポイントを作成
    endpoint_t *endpoint_temperature_sensor = temperature_sensor::create(node, &temperature_sensor_config, ENDPOINT_FLAG_NONE, NULL);
    endpoint_t *endpoint_humidity_sensor = humidity_sensor::create(node, &humidity_sensor_config, ENDPOINT_FLAG_NONE, NULL);
    endpoint_t *endpoint_pressure_sensor = pressure_sensor::create(node, &pressure_sensor_config, ENDPOINT_FLAG_NONE, NULL);

    Serial.println("Getting attribute references...");
    // 属性参照を保存 後で属性値を読み取るために使用
    attribute_ref_temperature = attribute::get(cluster::get(endpoint_temperature_sensor, CLUSTER_ID_TEMP), ATTRIBUTE_ID_TEMP);
    attribute_ref_humidity = attribute::get(cluster::get(endpoint_humidity_sensor, CLUSTER_ID_HUMID), ATTRIBUTE_ID_HUMID);
    attribute_ref_pressure = attribute::get(cluster::get(endpoint_pressure_sensor, CLUSTER_ID_PRESS), ATTRIBUTE_ID_PRESS);
    // 生成されたエンドポイントIDを保存
    temperature_sensor_endpoint_id = endpoint::get_id(endpoint_temperature_sensor);
    humidity_sensor_endpoint_id = endpoint::get_id(endpoint_humidity_sensor);
    pressure_sensor_endpoint_id = endpoint::get_id(endpoint_pressure_sensor);
    #endif
    #if ENABLE_SENSOR_2
    // デフォルト値でプラグインユニットエンドポイント/クラスター/属性をセットアップ
    light_sensor::config_t illuminance_sensor_config;
    occupancy_sensor::config_t occupancy_sensor_config;
    //　エンドポイントを作成
    endpoint_t *endpoint_illuminance_sensor = light_sensor::create(node, &illuminance_sensor_config, ENDPOINT_FLAG_NONE, NULL);
    endpoint_t *endpoint_occupancy_sensor = occupancy_sensor::create(node, &occupancy_sensor_config, ENDPOINT_FLAG_NONE, NULL);
    // 属性参照を保存。後で属性値を読み取るために使用。
    attribute_ref_illuminance = attribute::get(cluster::get(endpoint_illuminance_sensor, CLUSTER_ID_LIGHT), ATTRIBUTE_ID_LIGHT);
    attribute_ref_occupancy = attribute::get(cluster::get(endpoint_occupancy_sensor, CLUSTER_ID_OCCUP), ATTRIBUTE_ID_OCCUP);
    // 生成されたエンドポイントIDを保存
    illuminance_sensor_endpoint_id = endpoint::get_id(endpoint_illuminance_sensor);
    occupancy_sensor_endpoint_id = endpoint::get_id(endpoint_occupancy_sensor);
    #endif
    #if ENABLE_IR
    // エアコン
    // デフォルト値でプラグインユニットエンドポイント/クラスター/属性をセットアップ
    thermostat::config_t thermostat_config;
    thermostat_config.thermostat.local_temperature = 2500;
    thermostat_config.thermostat.system_mode = 0; // 0:Off, 1:Auto, 3:Cool, 4:Heat
    endpoint_t *endpoint_ac = thermostat::create(node, &thermostat_config, ENDPOINT_FLAG_NONE, NULL); //　エンドポイントを作成
    thermostat::add(endpoint_ac, &thermostat_config);
    ac_endpoint_id = endpoint::get_id(endpoint_ac); // 生成されたエンドポイントIDを保存
    // エアコン追加設定
    cluster_t *therm_cluster = cluster::get(endpoint_ac, CLUSTER_ID_AC);
    // units of temperature are in 0.01 Celsius for matter interactions
    cluster::thermostat::feature::cooling::config_t cooling_config;
    cooling_config.abs_max_cool_setpoint_limit = maxCoolingSetpoint;
    cooling_config.abs_min_cool_setpoint_limit = minCoolingSetpoint;
    cooling_config.max_cool_setpoint_limit = maxCoolingSetpoint;
    cooling_config.min_cool_setpoint_limit = minCoolingSetpoint;
    cooling_config.pi_cooling_demand = 0;
    cooling_config.occupied_cooling_setpoint = occupiedCoolingSetpoint;
    cluster::thermostat::feature::cooling::add(therm_cluster, &cooling_config);

    cluster::thermostat::feature::heating::config_t heating_config;
    heating_config.abs_max_heat_setpoint_limit = maxHeatingSetpoint;
    heating_config.abs_min_heat_setpoint_limit = minHeatingSetpoint;
    heating_config.max_heat_setpoint_limit = maxHeatingSetpoint;
    heating_config.min_heat_setpoint_limit = minHeatingSetpoint;
    heating_config.pi_heating_demand = 0;
    heating_config.occupied_heating_setpoint = occupiedHeatingSetpoint;
    cluster::thermostat::feature::heating::add(therm_cluster, &heating_config);

    // set heating step size
    matterValue = esp_matter_int16(10);  //10C 
    attribute::update(ac_endpoint_id, CLUSTER_ID_AC, clusters::Thermostat::Attributes::SetpointChangeAmount::Id, &matterValue);
    // set local temperature
    // matterValue = esp_matter_float(2500);  //25C
    // attribute::update(ac_endpoint_id, CLUSTER_ID_AC, clusters::Thermostat::Attributes::LocalTemperature::Id, &matterValue);
    // attribute::update(ac_endpoint_id, CLUSTER_ID_AC, clusters::Thermostat::Attributes::SetpointChangeAmount::Id, 10);

    esp_matter_attr_val_t setpoint_change_amount = esp_matter_invalid(NULL);
    attribute_t * attribute = attribute::get(cluster::get(endpoint_ac, CLUSTER_ID_AC), clusters::Thermostat::Attributes::SetpointChangeAmount::Id);
    attribute::get_val(attribute, &setpoint_change_amount);
    Serial.printf("SetpointChangeAmount: %d\n", setpoint_change_amount.val.i16);
    setpoint_change_amount = esp_matter_float(100);  //1C
    Serial.printf("SetpointChangeAmount: %d\n", setpoint_change_amount.val.i16);
    attribute::update(ac_endpoint_id, CLUSTER_ID_AC, clusters::Thermostat::Attributes::SetpointChangeAmount::Id, &setpoint_change_amount);

    #if ENABLE_IR_LIGHT
    // 照明
    color_temperature_light::config_t light_config;
    endpoint_t *endpoint_light = color_temperature_light::create(node, &light_config, ENDPOINT_FLAG_NONE, NULL);
    // attribute_ref_light = attribute::get(cluster::get(endpoint_light, CLUSTER_ID_ONOFF), ATTRIBUTE_ID_ONOFF);
    light_endpoint_id = endpoint::get_id(endpoint_light);
    #elif ENABLE_LIGHT_SWITCH
    // 照明
    color_dimmer_switch::config_t light_config;
    // on_off_switch::config_t light_config;
    // dimmer_switch::config_t light_config;
    // generic_switch::config_t light_config;
    // on_off_switch::config_t light_config;
    contact_sensor::config_t contact_sensor_config;
    contact_sensor_config.boolean_state.state_value = CONTACT_SENSOR_STATE_OPEN;
 
    endpoint_t *endpoint_light = color_dimmer_switch::create(node, &light_config, ENDPOINT_FLAG_NONE, NULL);
    // attribute_ref_light = attribute::get(cluster::get(endpoint_light, CLUSTER_ID_ONOFF), ATTRIBUTE_ID_ONOFF);
    light_endpoint_id = endpoint::get_id(endpoint_light);


    // スイッチのエンドポイントを作成
    endpoint_t *switch_endpoint = endpoint::create(node, ENDPOINT_FLAG_NONE);
    // OnOff クライアントを作成
    on_off::client_t *switch_cluster = on_off::client::create(switch_endpoint);
    #endif

    // 扇風機
    fan::config_t fan_config;
    // fan_config.fan.fan_mode = 0;
    // fan_config.fan.fan_mode_sequence = 0;
    // fan_config.fan.fan_mode_sequence_length = 0;
    // fan_config.fan.fan_mode_sequence_repeat = 0;

    endpoint_t *endpoint_fan = fan::create(node, &fan_config, ENDPOINT_FLAG_NONE, NULL); //　エンドポイントを作成
    // attribute_ref_fan = attribute::get(cluster::get(endpoint_fan, CLUSTER_ID_FAN), ATTRIBUTE_ID_FAN); // 属性参照を保存。後で属性値を読み取るために使用。
    fan_endpoint_id = endpoint::get_id(endpoint_fan); // 生成されたエンドポイントIDを保存
    #endif


    // DACをセットアップ（ここでカスタム委任データ、パスコードなどを設定するのが良い場所です）
    Serial.println("Setting up DAC...");
    esp_matter::set_custom_dac_provider(chip::Credentials::Examples::GetExampleDACProvider());

    // Matterデバイスを起動
    Serial.println("Starting Matter node...");
    esp_matter::start(on_device_event);

    // Matterデバイスのセットアップに必要なコードを印刷
    Serial.println("Printing onboarding codes...");
    PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));

    // // ESP-IDF のログレベルを ERROR のみに設定
    // esp_log_level_set("*", ESP_LOG_ERROR);
    // // esp_log_level_set("*", ESP_LOG_NONE);
    // // 特定のカテゴリのログを無効化
    // esp_log_level_set("chip", ESP_LOG_ERROR);
    // esp_log_level_set("DMG", ESP_LOG_ERROR);
    // esp_log_level_set("EM", ESP_LOG_ERROR);
    // esp_log_level_set("IN", ESP_LOG_ERROR);
    // esp_log_level_set("IM", ESP_LOG_ERROR);

    // // Matter のログを ERROR のみに制限
    // chip::Logging::SetLogFilter(chip::Logging::LogCategory::kLogCategory_Error);

    Serial.println("Matter device setup complete");
}

#endif // MATTER_FUNC_HPP_


// プラグインユニットのオン/オフ属性値を読み取ります
// esp_matter_attr_val_t get_onoff_attribute_value(esp_matter::attribute_t *attribute_) {
//     esp_matter_attr_val_t onoff_value = esp_matter_invalid(NULL);
//     attribute::get_val(attribute_ref, &onoff_value);
//     return onoff_value;
// }

// プラグインユニットのオン/オフ属性値を設定します
void set_onoff_attribute_value(esp_matter_attr_val_t *onoff_value, uint16_t plugin_unit_endpoint_id) {
//   attribute::update(plugin_unit_endpoint_id, CLUSTER_ID, ATTRIBUTE_ID, onoff_value);
}

void loop_matter(ClimateData &climate_data, LightData &light_data, AC_Data &ac_data, FanData &fan_data, CurtainData &curtain_data) {
    // トグルプラグインユニットボタンが押されたとき（デバウンス付き）、プラグインユニット属性値が変更されます
    // if ((millis() - last_toggle) > DEBOUNCE_DELAY) {
    //     if (!digitalRead(TOGGLE_BUTTON_PIN_1)) {
    //         last_toggle = millis();
    //         // 実際のオン/オフ値を読み取り、反転して設定
    //         esp_matter_attr_val_t onoff_value = get_onoff_attribute_value(attribute_ref_1);
    //         onoff_value.val.b = !onoff_value.val.b;
    //         set_onoff_attribute_value(&onoff_value, plugin_unit_endpoint_id_1);
    //     }

    //     if (!digitalRead(TOGGLE_BUTTON_PIN_2)) {
    //         last_toggle = millis();
    //         // 実際のオン/オフ値を読み取り、反転して設定
    //         esp_matter_attr_val_t onoff_value = get_onoff_attribute_value(attribute_ref_2);
    //         onoff_value.val.b = !onoff_value.val.b;
    //         set_onoff_attribute_value(&onoff_value, plugin_unit_endpoint_id_2);
    //     }
    // }

    // センサーの値が変更された場合、属性値を更新

    static ClimateData last_climate_data;
    if (last_climate_data.temperature != climate_data.temperature) {
        matterValue = esp_matter_int16(climate_data.temperature*10);
        attribute::update(temperature_sensor_endpoint_id, CLUSTER_ID_TEMP, ATTRIBUTE_ID_TEMP, &matterValue);
        last_climate_data.temperature = climate_data.temperature;
    }
    if (last_climate_data.humidity != climate_data.humidity) {
        matterValue = esp_matter_int16(climate_data.humidity*100);
        attribute::update(humidity_sensor_endpoint_id, CLUSTER_ID_HUMID, ATTRIBUTE_ID_HUMID, &matterValue);
        last_climate_data.humidity = climate_data.humidity;
    }
    if (last_climate_data.pressure != climate_data.pressure) {
        matterValue = esp_matter_int16(climate_data.pressure);
        attribute::update(pressure_sensor_endpoint_id, CLUSTER_ID_PRESS, ATTRIBUTE_ID_PRESS, &matterValue);
        last_climate_data.pressure = climate_data.pressure;
    }

    // static LightData last_light_data;

    static AC_Data last_ac_data;
    if (last_ac_data.mode != ac_data.mode) {
        matterValue = esp_matter_uint8((uint8_t)ac_data.mode); // 0:Off, 1:Auto, 3:Cool, 4:Heat
        attribute::update(ac_endpoint_id, CLUSTER_ID_AC, clusters::Thermostat::Attributes::SystemMode::Id, &matterValue);
        last_ac_data.mode = ac_data.mode;
    }
    if (last_ac_data.temp != ac_data.temp) {
        matterValue = esp_matter_uint8(ac_data.temp);
        switch ((uint8_t)ac_data.mode) {
            case (uint8_t)ac::SystemMode::Cool:
                attribute::update(ac_endpoint_id, CLUSTER_ID_AC, clusters::Thermostat::Attributes::OccupiedCoolingSetpoint::Id, &matterValue);
                break;
            case (uint8_t)ac::SystemMode::Heat:
                attribute::update(ac_endpoint_id, CLUSTER_ID_AC, clusters::Thermostat::Attributes::OccupiedHeatingSetpoint::Id, &matterValue);
                break;
            default:
                break;
        }
        last_ac_data.temp = ac_data.temp;
    }

    static FanData last_fan_data;
    if (last_fan_data.power != fan_data.power) {
        matterValue = esp_matter_uint8((uint8_t)fan_data.power);
        attribute::update(fan_endpoint_id, CLUSTER_ID_FAN, clusters::FanControl::Attributes::FanMode::Id, &matterValue);
        last_fan_data.power = fan_data.power;
    }
    // if (last_fan_data. != fan_data.fanModeSequence) {
    //     matterValue = esp_matter_uint8(fan_data.fanModeSequence);
    //     attribute::update(fan_endpoint_id, CLUSTER_ID_FAN, clusters::FanControl::Attributes::FanModeSequence::Id, &matterValue);
    //     last_fan_data.fanModeSequence = fan_data.fanModeSequence;
    // }
    if (last_fan_data.speed != fan_data.speed) {
        matterValue = esp_matter_uint8(fan_data.speed*8);
        attribute::update(fan_endpoint_id, CLUSTER_ID_FAN, clusters::FanControl::Attributes::PercentSetting::Id, &matterValue);
        last_fan_data.speed = fan_data.speed;
    }

    // static CurtainData last_curtain_data;
    // if (last_curtain_data.position != curtain_data.position) {
    //     matterValue = esp_matter_uint8(curtain_data.position);
    //     attribute::update(curtain_data.endpoint_id, curtain_data.cluster_id, curtain_data.attribute_id, &matterValue);
    //     last_curtain_data.position = curtain_data.position;
    // }


    esp_log_level_set("*", ESP_LOG_ERROR);
    
}