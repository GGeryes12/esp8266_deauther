#pragma once

#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <PubSubClient.h>

class MQTTManager {
public:
    MQTTManager();
    void setup(const char* server, int port, const char* user = "", const char* password = "");
    void connect();
    void publishMessage(const char* topic, const char* message);
    void subscribe(const char* topic);
    void loop();
    void setCallback(std::function<void(char*, byte*, unsigned int)> callback);
    
    PubSubClient* client;
        // New functions for reconnecting and non-blocking loop handling
        void handleMQTT();
        void reconnectMQTT();

private:
    WiFiClient espClient;
    const char* mqttUser;
    const char* mqttPassword;
};
