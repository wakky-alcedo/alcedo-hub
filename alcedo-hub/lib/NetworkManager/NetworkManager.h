#pragma once
#include <WiFi.h>
#include <PubSubClient.h>
#include <functional>

class NetworkManager {
public:
    using MqttCallback = std::function<void(String, String)>;

    NetworkManager();
    void setup(const char* ssid, const char* pass, const char* mqttServer);
    void loop();
    void publish(const char* topic, const char* payload);
    void subscribe(const char* topic);
    void setCallback(MqttCallback cb);
    void _handleInternalMqttMessage(char* topic, byte* payload, unsigned int length);

private:
    void _connectWifi();
    void _reconnectMqtt();
    
    WiFiClient _espClient;
    PubSubClient _mqttClient;
    
    const char* _ssid;
    const char* _pass;
    const char* _mqttServer;
    
    MqttCallback _onMessage;
};