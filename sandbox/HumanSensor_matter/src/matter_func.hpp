#ifndef MATTER_FUNC_HPP_
#define MATTER_FUNC_HPP_

#include <Arduino.h>
#include "Matter.h"
#include <app/server/OnboardingCodesUtil.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include "matter_on_update.hpp"
using namespace chip;
namespace clusters = chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::endpoint;

#define CHIP_LOG_LEVEL CHIP_LOG_LEVEL_ERROR // エラーレベルのログのみ出力

constexpr uint8_t HUMAN_SENSOR_PIN = D10;
bool last_human_sensor_state = false;

// Matterプラグインユニットデバイスで使用されるクラスターと属性ID
// Matterデバイスに割り当てられるエンドポイントと属性参照
const uint32_t CLUSTER_ID_LIGHT = clusters::IlluminanceMeasurement::Id;
const uint32_t CLUSTER_ID_OCCUP = clusters::OccupancySensing::Id;
const uint32_t ATTRIBUTE_ID_LIGHT = clusters::IlluminanceMeasurement::Attributes::MeasuredValue::Id;
const uint32_t ATTRIBUTE_ID_OCCUP = clusters::OccupancySensing::Attributes::Occupancy::Id;
uint16_t illuminance_sensor_endpoint_id = 0; // 照度センサ
uint16_t occupancy_sensor_endpoint_id = 0; // 占有センサ(人感センサ)
attribute_t *attribute_ref_illuminance;
attribute_t *attribute_ref_occupancy;


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
    // if (type == attribute::PRE_UPDATE) {
    // }
    return ESP_OK;
}

/**
 * @brief Matterデバイスのセットアップ
 * 
 */
void setup_matter() {
    Serial.println("Start to setup Matter device");

    // デバッグログを有効にする
    // esp_log_level_set("*", ESP_LOG_DEBUG);
    // chip::DeviceLayer::PlatformMgr().InitChipStack();

    // Matterノードをセットアップ
    Serial.println("Creating Matter node");
    node::config_t node_config;
    node_t *node = node::create(&node_config, on_attribute_update, on_identification);

    // on_off_plugin_unit::config_t plugin_unit_config;
    // plugin_unit_config.on_off.on_off = false;
    // plugin_unit_config.on_off.lighting.start_up_on_off = false;

    // デフォルト値でプラグインユニットエンドポイント/クラスター/属性をセットアップ
    Serial.println("Setting up plugin unit endpoints...");
    light_sensor::config_t illuminance_sensor_config;
    occupancy_sensor::config_t occupancy_sensor_config;
    //　エンドポイントを作成
    endpoint_t *endpoint_illuminance_sensor = light_sensor::create(node, &illuminance_sensor_config, ENDPOINT_FLAG_NONE, NULL);
    endpoint_t *endpoint_occupancy_sensor = occupancy_sensor::create(node, &occupancy_sensor_config, ENDPOINT_FLAG_NONE, NULL);
    // // 属性参照を保存。後で属性値を読み取るために使用。
    // attribute_ref_illuminance = attribute::get(cluster::get(endpoint_illuminance_sensor, CLUSTER_ID_LIGHT), ATTRIBUTE_ID_LIGHT);
    // attribute_ref_occupancy = attribute::get(cluster::get(endpoint_occupancy_sensor, CLUSTER_ID_OCCUP), ATTRIBUTE_ID_OCCUP);
    // // 生成されたエンドポイントIDを保存
    // illuminance_sensor_endpoint_id = endpoint::get_id(endpoint_illuminance_sensor);
    // occupancy_sensor_endpoint_id = endpoint::get_id(endpoint_occupancy_sensor);

    // DACをセットアップ（ここでカスタム委任データ、パスコードなどを設定するのが良い場所です）
    Serial.println("Setting up custom DAC provider");
    esp_matter::set_custom_dac_provider(chip::Credentials::Examples::GetExampleDACProvider());

    // Matterデバイスを起動
    Serial.println("Starting Matter device");
    esp_matter::start(on_device_event);

    Serial.println("Matter device setup save");
    // 属性参照を保存。後で属性値を読み取るために使用。
    attribute_ref_illuminance = attribute::get(cluster::get(endpoint_illuminance_sensor, CLUSTER_ID_LIGHT), ATTRIBUTE_ID_LIGHT);
    attribute_ref_occupancy = attribute::get(cluster::get(endpoint_occupancy_sensor, CLUSTER_ID_OCCUP), ATTRIBUTE_ID_OCCUP);
    // 生成されたエンドポイントIDを保存
    illuminance_sensor_endpoint_id = endpoint::get_id(endpoint_illuminance_sensor);
    occupancy_sensor_endpoint_id = endpoint::get_id(endpoint_occupancy_sensor);


    // Matterデバイスのセットアップに必要なコードを印刷
    Serial.println("Printing onboarding codes...");
    PrintOnboardingCodes(chip::RendezvousInformationFlags(chip::RendezvousInformationFlag::kBLE));

    // ESP-IDF のログレベルを ERROR のみに設定
    esp_log_level_set("*", ESP_LOG_ERROR);
    // esp_log_level_set("*", ESP_LOG_NONE);
    // 特定のカテゴリのログを無効化
    esp_log_level_set("chip", ESP_LOG_ERROR);
    esp_log_level_set("DMG", ESP_LOG_ERROR);
    esp_log_level_set("EM", ESP_LOG_ERROR);
    esp_log_level_set("IN", ESP_LOG_ERROR);
    esp_log_level_set("IM", ESP_LOG_ERROR);

    // Matter のログを ERROR のみに制限
    chip::Logging::SetLogFilter(chip::Logging::LogCategory::kLogCategory_Error);

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

void loop_matter() {
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
    if (digitalRead(HUMAN_SENSOR_PIN) == HIGH) {
        esp_matter_attr_val_t onoff_value = esp_matter_invalid(NULL);
        attribute::get_val(attribute_ref_occupancy, &onoff_value);
        onoff_value.val.b = true;
        attribute::update(occupancy_sensor_endpoint_id, CLUSTER_ID_OCCUP, ATTRIBUTE_ID_OCCUP, &onoff_value);
        if (last_human_sensor_state == false) {
            Serial.println("HIGH");
            last_human_sensor_state = true;
        }
    } else {
        esp_matter_attr_val_t onoff_value = esp_matter_invalid(NULL);
        attribute::get_val(attribute_ref_occupancy, &onoff_value);
        onoff_value.val.b = false;
        attribute::update(occupancy_sensor_endpoint_id, CLUSTER_ID_OCCUP, ATTRIBUTE_ID_OCCUP, &onoff_value);
        if (last_human_sensor_state == true) {
            Serial.println("LOW");
            last_human_sensor_state = false;
        }
    }
    // esp_log_level_set("*", ESP_LOG_ERROR);
    delay(1000);
}