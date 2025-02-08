#include <Arduino.h>

#include "ClimateSensor.hpp"

ClimateSensor sensor;
ClimateData climate_data;

void setup(void) {
    Serial.begin(115200);
    Serial.flush();

    // AHT10/AHT20の初期化
    // while (!Serial)
    //     delay(10); // will pause Zero, Leonardo, etc until serial console opens

    sensor.begin();
}

void loop(void) {
    // センサからデータを取得
    sensor.read(climate_data);

    // シリアルモニタに表示
    Serial.print("Temperature = ");
    Serial.print(climate_data.temperature/10.0f);
    Serial.println(" *C");
    Serial.print("Humidity = ");
    Serial.print(climate_data.humidity);
    Serial.println(" % rH"); // humidity is measured in % relative humidity (% rH)
    Serial.print("Pressure = ");
    Serial.print(climate_data.pressure);
    Serial.println(" hPa");
    Serial.println("---------------------------------");

    // 1秒待つ
    delay(1000);
}
