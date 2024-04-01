#include <PubSubClient.h>
#include <WiFiNINA.h>

// Replace with your own WiFi and MQTT information
const char* ssid = "VIVO";
const char* password = "sanjiv1458";
const char* mqtt_server = "c18d3fe8891a42b8b338f6f8d5a2ee0d.s1.eu.hivemq.cloud";
const char* mqtt_username = "sanjiv4853";
const char* mqtt_password = "Sanjivsingh@123";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

// Define the IR lm393 sensor pins
const int sensorPin1 = 2;
const int sensorPin2 = 6;

void setup() {
  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Connect to MQTT broker
  mqttClient.setServer(mqtt_server, 8884);
  while (!mqttClient.connected()) {
    if (mqttClient.connect("arduino", mqtt_username, mqtt_password)) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }

  // Set the sensor pins as inputs
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
}

void loop() {
  // Read the sensor values
  int sensorValue1 = digitalRead(sensorPin1);
  int sensorValue2 = digitalRead(sensorPin2);

  // Publish the sensor values to MQTT
  String payload = String(sensorValue1) + "," + String(sensorValue2);
  mqttClient.publish("sensors/ir", payload.c_str());

  // Wait for 1 second before reading again
  delay(1000);

  // Call the MQTT client loop function to receive messages
  mqttClient.loop();
}
