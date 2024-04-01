#include <WiFiNINA.h>
#include <PubSubClient.h>

// WiFi credentials
const char* ssid = "VIVO";
const char* password = "sanjiv1458";

// MQTT broker details
const char* broker = "broker.hivemq.com";
const int port = 8883;
const char* clientId = "nano33-client";
const char* topic = "sensor/status";
const char* username = "sanjiv4853";
const char* password_mqtt = "Sanjivsingh@123";

// LM393 IR Sensor pin
const int sensorPin = A0;

// WiFi and MQTT objects
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  // Initialize Serial port
  Serial.begin(9600);

  // Connect to WiFi
  connectToWiFi();

  // Connect to MQTT broker
  mqttClient.setServer(broker, port);
  mqttClient.setCallback(onMessageReceived);
  connectToMqtt();
}

void loop() {
  // Read sensor value
  int sensorValue = analogRead(sensorPin);

  // Send sensor status to MQTT broker
  if (sensorValue > 512) {
    mqttClient.publish(topic, "ON");
  } else {
    mqttClient.publish(topic, "OFF");
  }

  // Wait for 1 second before reading sensor again
  delay(1000);

  // Keep MQTT connection alive
  if (!mqttClient.connected()) {
    connectToMqtt();
  }
  mqttClient.loop();
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void connectToMqtt() {
  Serial.println("Connecting to MQTT broker...");
  while (!mqttClient.connected()) {
    if (mqttClient.connect(clientId, username, password_mqtt)) {
      mqttClient.subscribe(topic);
      Serial.println("Connected to MQTT broker");
    } else {
      delay(1000);
      Serial.println("Connecting to MQTT broker...");
    }
  }
}

void onMessageReceived(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Message payload: ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
