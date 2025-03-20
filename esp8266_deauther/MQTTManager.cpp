#include "MQTTManager.h"

MQTTManager::MQTTManager() {
    client = new PubSubClient(espClient);
}

void MQTTManager::setup(const char* server, int port, const char* user, const char* password) {
    mqttUser = user;
    mqttPassword = password;
    client->setServer(server, port);
}

void MQTTManager::connect() {
    Serial.print("Attempting MQTT connection...");

    if (mqttUser && mqttPassword) {
        if (client->connect("DSTIKE_Client", mqttUser, mqttPassword)) {
            Serial.println("MQTT connected");
        }
    } else {
        if (client->connect("DSTIKE_Client")) {
            Serial.println("MQTT connected");
        }
    }

    if (client->connected()) {
        subscribe("dstike/send");
    } else {
        Serial.print("MQTT connection failed, state = ");
        Serial.println(client->state());
    }
}




void MQTTManager::publishMessage(const char* topic, const char* message) {
    if (client->connected()) {
        client->publish(topic, message);
    } else {
        Serial.println("MQTT not connected, can't publish");
    }
}

void MQTTManager::subscribe(const char* topic) {
    if (client->connected()) {
        client->subscribe(topic);
    } else {
        Serial.println("MQTT not connected, can't subscribe");
    }
}

void MQTTManager::loop() {
    if (client->connected()) {
        client->loop();
    }
}


void MQTTManager::setCallback(std::function<void(char*, byte*, unsigned int)> callback) {
    client->setCallback(callback);
}

void MQTTManager::handleMQTT() {
    static unsigned long lastMQTTCheck = 0;
    const unsigned long mqttInterval = 500; // Check every 500 ms

    if (WiFi.status() == WL_CONNECTED) { // ✅ Only try MQTT when WiFi is connected
        if (millis() - lastMQTTCheck >= mqttInterval) {
            lastMQTTCheck = millis();

            if (client->connected()) {
                client->loop();
            } else {
                //reconnectMQTT(); // Try to reconnect in the background
            }
        }
    }
}



void MQTTManager::reconnectMQTT() {
    static unsigned long lastReconnectAttempt = 0;
    const unsigned long reconnectInterval = 5000; // Retry every 5 seconds

    if (!client->connected() && millis() - lastReconnectAttempt >= reconnectInterval) {
        lastReconnectAttempt = millis();
        Serial.println("Attempting MQTT reconnect...");

        if (mqttUser && mqttPassword) {
            if (client->connect("DSTIKE_Client", mqttUser, mqttPassword)) {
                Serial.println("MQTT reconnected");
            }
        } else {
            if (client->connect("DSTIKE_Client")) {
                Serial.println("MQTT reconnected");
            }
        }

        if (client->connected()) {
            subscribe("dstike/send"); // Resubscribe to topics after reconnect
        } else {
            Serial.println("MQTT reconnect failed.");
        }
    }
}

