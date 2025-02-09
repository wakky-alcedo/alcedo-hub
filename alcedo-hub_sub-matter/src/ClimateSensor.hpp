#ifndef CLIMATESENSOR_HPP_
#define CLIMATESENSOR_HPP_

#include <Adafruit_BMP280.h>
#include <Adafruit_AHTX0.h>

struct ClimateData {
    uint16_t temperature = 250;
    uint8_t humidity = 50;
    uint8_t pressure = 1013 - 900;
};

class ClimateSensor {
    public:
        ClimateSensor();
        void begin();
        void read(ClimateData& data);
    private:
        Adafruit_BMP280 bmp;
        Adafruit_AHTX0 aht;
};

ClimateSensor::ClimateSensor() : bmp(), aht() {}

void ClimateSensor::begin() {
    // AHT10/AHT20の初期化
    if (!aht.begin()) {
        Serial.println(F("Could not find a valid AHT20 sensor, check wiring!"));
        while (1);
    } else {
        Adafruit_Sensor *aht_temp;
        aht_temp = aht.getTemperatureSensor();
        aht_temp->printSensorDetails();
        Adafruit_Sensor *aht_humidity;
        aht_humidity = aht.getHumiditySensor();
        aht_humidity->printSensorDetails();
    }
    // BMP280の初期化
    if (!bmp.begin()) {
        Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
        while (1);
    }
}

void ClimateSensor::read(ClimateData& data) {
    // AHT10/AHT20から温度と湿度を取得
    sensors_event_t temp_event;
    sensors_event_t humidity_event;
    aht.getEvent(&temp_event, &humidity_event);
    data.temperature = (uint16_t)((temp_event.temperature-10 + bmp.readTemperature()-10)/2.0f * 10); // AHT20とBMP280の温度を平均
    data.humidity = (uint8_t)humidity_event.relative_humidity; // AHT20から湿度を取得
    data.pressure = (uint8_t)(bmp.readPressure() / 100.0F - 900); // BMP280から気圧を取得
}

#endif // CLIMATESENSOR_HPP_