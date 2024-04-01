#include <WiFiNINA.h> // WiFi library
#include <PubSubClient.h> // MQTT library

// WiFi network credentials
const char* ssid = "VIVO";
const char* password = "sanjiv1458";

// MQTT broker information
const char* mqtt_server = "broker.hivemq.com";
const int mqtt_port = 1883;
const char* mqtt_topic = "PARKING_SYSTEM_DATA";

// Sensor pins
#define SENSOR1_PIN 2
#define SENSOR2_PIN 3
#define SENSOR3_PIN 4
#define SENSOR4_PIN 5
#define SENSOR5_PIN 6
#define SENSOR6_PIN 7
// Sensor values
int sensor1Value = 0;
int sensor2Value = 0;
int sensor3Value = 0;
int sensor4Value = 0;
int sensor5Value = 0;
int sensor6Value = 0;

// MQTT client object
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(115200);
  pinMode(SENSOR1_PIN, INPUT_PULLUP);
  pinMode(SENSOR2_PIN, INPUT_PULLUP);
  pinMode(SENSOR3_PIN, INPUT_PULLUP);
  pinMode(SENSOR4_PIN, INPUT_PULLUP);
  pinMode(SENSOR5_PIN, INPUT_PULLUP);
  pinMode(SENSOR6_PIN, INPUT_PULLUP);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  mqttClient.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (mqttClient.connect("nodeMCU")) {
      Serial.println("Connected to MQTT broker");
    } else {
      Serial.println("Connection failed");
      delay(5000);
      return;
    }
  }

  sensor1Value = digitalRead(SENSOR1_PIN);
  sensor2Value = digitalRead(SENSOR2_PIN);
  sensor3Value = digitalRead(SENSOR3_PIN);
  sensor4Value = digitalRead(SENSOR4_PIN);
  sensor5Value = digitalRead(SENSOR5_PIN);
  sensor6Value = digitalRead(SENSOR6_PIN);

  String payload = String(sensor1Value) + "," + String(sensor2Value) + "," + String(sensor3Value) + "," + String(sensor4Value) + "," + String(sensor5Value) + "," + String(sensor6Value);

  mqttClient.publish(mqtt_topic, payload.c_str());
  mqttClient.loop();
  delay(10);
}
