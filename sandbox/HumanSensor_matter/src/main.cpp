/**
 * @file TwoEndpointsPluginUnit.cpp
 * @brief Matterプラグインユニット（スイッチ）デバイスの例を示すプログラム。
 * 
 * このプログラムは、Matterとトグルボタンを使用してLEDを制御する2つのエンドポイントを持つOnOffクラスターを備えたMatterプラグインユニット（スイッチ）デバイスの例を示します。
 * 
 * @details
 * プラグインユニットは以下の方法でトグルできます:
 *  - Matter（CHIPToolや他のMatterコントローラーを介して）
 *  - トグルボタン（デバウンス付き）
 * 
 * @note PINの設定が必要です。
 * 
 * @section pins ピン設定
 * - LED_PIN_1: D0
 * - LED_PIN_2: D1
 * - TOGGLE_BUTTON_PIN_1: D9
 * - TOGGLE_BUTTON_PIN_2: D8
 * 
 * @section debounce デバウンス設定
 * - DEBOUNCE_DELAY: 500ms
 * 
 * @section clusters クラスターと属性ID
 * - CLUSTER_ID: OnOff::Id
 * - ATTRIBUTE_ID: OnOff::Attributes::OnOff::Id
 * 
 * @section endpoints エンドポイントと属性参照
 * - plugin_unit_endpoint_id_1: エンドポイント1のID
 * - plugin_unit_endpoint_id_2: エンドポイント2のID
 * - attribute_ref_1: エンドポイント1の属性参照
 * - attribute_ref_2: エンドポイント2の属性参照
 * 
 * @section functions 関数
 * - setup(): 初期設定を行います。
 * - loop(): トグルボタンの状態を監視し、デバウンス処理を行います。
 * - on_device_event(): デバイスイベントのリスナー（空の実装）。
 * - on_identification(): デバイス識別のコールバック。
 * - on_attribute_update(): 属性更新リクエストのリスナー。
 * - get_onoff_attribute_value(): プラグインユニットのOn/Off属性値を読み取ります。
 * - set_onoff_attribute_value(): プラグインユニットのOn/Off属性値を設定します。
 * 
 * @section usage 使用方法
 * 1. 必要なPINを設定します。
 * 2. setup()関数を呼び出して初期設定を行います。
 * 3. loop()関数をメインループで呼び出し、トグルボタンの状態を監視します。
 */
#include <Arduino.h>
#include "Matter.h"
#include <app/server/OnboardingCodesUtil.h>
#include <credentials/examples/DeviceAttestationCredsExample.h>
#include "matter_func.hpp"
using namespace chip;
using namespace chip::app::Clusters;
using namespace esp_matter;
using namespace esp_matter::endpoint;

// PINを設定してください
const int LED_PIN_1 = D0;
const int LED_PIN_2 = D1;
const int TOGGLE_BUTTON_PIN_1 = D9;
const int TOGGLE_BUTTON_PIN_2 = D8;

void setup() {
    delay(5000);
    Serial.begin(115200);
    Serial.flush();
    delay(1000);
    
    pinMode(HUMAN_SENSOR_PIN, INPUT);
      
    pinMode(LED_PIN_1, OUTPUT);
    pinMode(LED_PIN_2, OUTPUT);
    pinMode(TOGGLE_BUTTON_PIN_1, INPUT);
    pinMode(TOGGLE_BUTTON_PIN_2, INPUT);

    setup_matter(); // Matterデバイスのセットアップ
}

// // プラグインユニットのオン/オフ属性値を読み取ります
// esp_matter_attr_val_t get_onoff_attribute_value(esp_matter::attribute_t *attribute_ref) {
//   esp_matter_attr_val_t onoff_value = esp_matter_invalid(NULL);
//   attribute::get_val(attribute_ref, &onoff_value);
//   return onoff_value;
// }

// // プラグインユニットのオン/オフ属性値を設定します
// void set_onoff_attribute_value(esp_matter_attr_val_t *onoff_value, uint16_t plugin_unit_endpoint_id) {
//   attribute::update(plugin_unit_endpoint_id, CLUSTER_ID, ATTRIBUTE_ID, onoff_value);
// }

// トグルプラグインユニットボタンが押されたとき（デバウンス付き）、プラグインユニット属性値が変更されます
void loop() {
    loop_matter();
//   if ((millis() - last_toggle) > DEBOUNCE_DELAY) {
//   if (!digitalRead(TOGGLE_BUTTON_PIN_1)) {
//     last_toggle = millis();
//     // 実際のオン/オフ値を読み取り、反転して設定
//     esp_matter_attr_val_t onoff_value = get_onoff_attribute_value(attribute_ref_1);
//     onoff_value.val.b = !onoff_value.val.b;
//     set_onoff_attribute_value(&onoff_value, plugin_unit_endpoint_id_1);
//   }

//   if (!digitalRead(TOGGLE_BUTTON_PIN_2)) {
//     last_toggle = millis();
//     // 実際のオン/オフ値を読み取り、反転して設定
//     esp_matter_attr_val_t onoff_value = get_onoff_attribute_value(attribute_ref_2);
//     onoff_value.val.b = !onoff_value.val.b;
//     set_onoff_attribute_value(&onoff_value, plugin_unit_endpoint_id_2);
//   }
//   }
}
