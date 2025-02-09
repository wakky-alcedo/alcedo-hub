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
#include "ClimateSensor.hpp"


// PINを設定してください
// const int LED_PIN_1 = 2;
// const int LED_PIN_2 = 4;
// const int TOGGLE_BUTTON_PIN_1 = 0;
// const int TOGGLE_BUTTON_PIN_2 = 15;
// constexpr uint8_t SPI_CS_PIN = 5;

ClimateSensor sensor;
ClimateData climate_data;

// トグルボタンのデバウンス
const int DEBOUNCE_DELAY = 500;
int last_toggle;

void setup() {
    delay(5000);
    Serial.begin(115200);
    Serial.flush();
    delay(1000);

    // pinMode(LED_PIN_1, OUTPUT);
    // pinMode(LED_PIN_2, OUTPUT);
    // pinMode(TOGGLE_BUTTON_PIN_1, INPUT);
    // pinMode(TOGGLE_BUTTON_PIN_2, INPUT);
    // pinMode(IR_SEND_PIN, OUTPUT);
    
    setup_matter(); // Matterデバイスのセットアップ
    
    sensor.begin();
}


void loop() {
    sensor.read(climate_data);

    loop_matter(climate_data);
}
