#include <Arduino.h>

#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>

Adafruit_AHTX0 aht;
Adafruit_Sensor *aht_humidity, *aht_temp;

Adafruit_BMP280 bmp;


void setup(void) {
    Serial.begin(115200);

    // AHT10/AHT20の初期化
    while (!Serial)
        delay(10); // will pause Zero, Leonardo, etc until serial console opens

    Serial.println("Adafruit AHT10/AHT20 test!");

    if (!aht.begin()) {
        Serial.println("Failed to find AHT10/AHT20 chip");
        while (1) {
        delay(10);
        }
    }

    Serial.println("AHT10/AHT20 Found!");
    aht_temp = aht.getTemperatureSensor();
    aht_temp->printSensorDetails();

    aht_humidity = aht.getHumiditySensor();
    aht_humidity->printSensorDetails();
    
    // BMP280の初期化
    bmp.begin();
}

uint32_t count = ~0;
void loop(void) {
    // 温度と湿度を取得
    sensors_event_t humidity, temp;
    aht.getEvent(&humidity, &temp); // AHT21から湿度と温度の取得
    float temperature1 = temp.temperature;
    float relative_humidity = humidity.relative_humidity;
    // aht_humidity->getEvent(&humidity);
    // aht_temp->getEvent(&temp);

    // 温度と気圧を取得
    bmp.takeForcedMeasurement();
    float temperature2 = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0F;

    // シリアルモニタに表示
    Serial.print("Temperature1 = ");
    Serial.print(temperature1);
    Serial.println(" *C");
    Serial.print("Humidity = ");
    Serial.print(relative_humidity);
    Serial.println(" % rH"); // humidity is measured in % relative humidity (% rH)
    Serial.print("Temperature2 = ");
    Serial.print(temperature2);
    Serial.println(" *C");
    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" hPa");

    // 1秒待つ
    delay(1000);
}
