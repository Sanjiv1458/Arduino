#include <Wire.h>
#include <Adafruit_Fingerprint.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

#define WIFI_SSID "your_SSID"
#define WIFI_PASSWORD "your_PASSWORD"
#define MQTT_SERVER "broker.hivemq.com"
#define MQTT_PORT 1883
#define MQTT_TOPIC "attendance"
#define MQTT_TOPIC_SET "set-fingerprint"
#define MQTT_TOPIC_STOP "attendance_control"

Adafruit_Fingerprint finger = Adafruit_Fingerprint(&Wire);
WiFiClient wifiClient;
PubSubClient client(wifiClient);

bool fingerprintMode = false;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  finger.begin(0x55);

  connectToWiFi();
  connectToMQTT();
}

void loop() {
  if (fingerprintMode) {
    getFingerprint();
  }
  client.loop();
}

void getFingerprint() {
  uint8_t id = finger.getImage();
  if (id == FINGERPRINT_OK) {
    id = finger.fingerFastSearch();
    if (id == FINGERPRINT_OK) {
      Serial.print("ID found: ");
      Serial.println(id);
      sendAttendanceData(id);
    }
  }
}

void sendAttendanceData(uint8_t id) {
  char attendanceMessage[10];
  sprintf(attendanceMessage, "%d", id);
  client.publish(MQTT_TOPIC, attendanceMessage);
}

void setFingerprint(uint8_t id) {
  Serial.print("Place your finger on the sensor to set fingerprint for ID ");
  Serial.println(id);
  while (finger.getImage() != FINGERPRINT_OK);
  finger.storeModel(id);
  Serial.print("Fingerprint set on index ");
  Serial.println(id);
}

void connectToWiFi() {
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Connecting to WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    delay(5000);
  }
  Serial.println("Connected to WiFi");
}

void connectToMQTT() {
  client.setServer(MQTT_SERVER, MQTT_PORT);
  client.setCallback(callback);
  while (!client.connected()) {
    Serial.print("Connecting to MQTT broker...");
    if (client.connect("arduino-client")) {
      client.subscribe(MQTT_TOPIC_SET);
      client.subscribe(MQTT_TOPIC_STOP);
      Serial.println("Connected to MQTT broker");
    }
    else {
      Serial.print("Failed to connect to MQTT broker. Error code: ");
      Serial.println(client.state());
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  payload[length] = '\0';
  int id = atoi((char*) payload);

  if (strcmp(topic, MQTT_TOPIC_SET) == 0) {
    setFingerprint(id);
  }
  else if (strcmp(topic, MQTT_TOPIC_STOP) == 0) {
    fingerprintMode = false;
  }
  else {
    Serial.println("Invalid topic");
  }
}
