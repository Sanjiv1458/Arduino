#include <WiFiNINA.h>
#include <PubSubClient.h>

// Wi-Fi and MQTT settings
const char* ssid = "Sanchit";
const char* password = "12345678";
const char* mqttServer = "broker.hivemq.com"; // HiveMQ broker address
const int mqttPort = 1883; // HiveMQ broker port

// LED Pins for Traffic Light 1
const int red1Pin = 2;
const int yellow1Pin = 3;
const int green1Pin = 4;

// LED Pins for Traffic Light 2
const int red2Pin = 5;
const int yellow2Pin = 6;
const int green2Pin = 7;

// Initialize WiFi and MQTT client
WiFiClient wifiClient;
PubSubClient client(wifiClient);

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Set the LED pins as outputs
  pinMode(red1Pin, OUTPUT);
  pinMode(yellow1Pin, OUTPUT);
  pinMode(green1Pin, OUTPUT);
  
  pinMode(red2Pin, OUTPUT);
  pinMode(yellow2Pin, OUTPUT);
  pinMode(green2Pin, OUTPUT);

  // Connect to MQTT broker
  client.setServer(mqttServer, mqttPort);
  client.setCallback(callback);

  // Connect to MQTT server
  while (!client.connected()) {
    Serial.println("Connecting to MQTT...");
    if (client.connect("arduino-client")) {
      Serial.println("Connected to MQTT");
      client.subscribe("traffic-light-1");
      client.subscribe("traffic-light-2");
    } else {
      Serial.println("MQTT connection failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.println("MQTT message received.");
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }

  if (strcmp(topic, "traffic-light-1") == 0) {
    controlTrafficLight(1, message);
  } else if (strcmp(topic, "traffic-light-2") == 0) {
    controlTrafficLight(2, message);
  }
}

void controlTrafficLight(int lightNumber, String message) {
  int redPin, yellowPin, greenPin;

  if (lightNumber == 1) {
    redPin = red1Pin;
    yellowPin = yellow1Pin;
    greenPin = green1Pin;
  } else {
    redPin = red2Pin;
    yellowPin = yellow2Pin;
    greenPin = green2Pin;
  }

  if (message == "RED") {
    digitalWrite(redPin, HIGH);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, LOW);
  } else if (message == "YELLOW") {
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, HIGH);
    digitalWrite(greenPin, LOW);
  } else if (message == "GREEN") {
    digitalWrite(redPin, LOW);
    digitalWrite(yellowPin, LOW);
    digitalWrite(greenPin, HIGH);
  }
}

void reconnect() {
  while (!client.connected()) {
    Serial.println("Reconnecting to MQTT...");
    if (client.connect("arduino-client")) {
      Serial.println("Connected to MQTT");
      client.subscribe("traffic-light-1");
      client.subscribe("traffic-light-2");
    } else {
      Serial.println("MQTT connection failed, retrying in 5 seconds...");
      delay(5000);
    }
  }
}

