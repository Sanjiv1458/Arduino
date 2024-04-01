#include <WiFiNINA.h>
#include <PubSubClient.h>

#define IR_SENSOR_PIN_1 2
#define IR_SENSOR_PIN_2 3
#define IR_SENSOR_PIN_3 4
#define IR_SENSOR_PIN_4 5
#define IR_SENSOR_PIN_5 6
#define IR_SENSOR_PIN_6 7

char ssid[] = "VIVO";    // your network SSID (name)
char pass[] = "sanjiv1458"; // your network password
int status = WL_IDLE_STATUS;      // the WiFi status
WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(9600);

  // attempt to connect to WiFi network
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    delay(5000);
  }

  Serial.println("Connected to wifi");
  mqttClient.setServer("broker.hivemq.com", 1883);
  mqttClient.setCallback(callback);

  connectToMqttBroker(); // Add this line to call the function
}

void loop() {
  // read IR sensor status
  int irSensor1Status = digitalRead(IR_SENSOR_PIN_1);
  int irSensor2Status = digitalRead(IR_SENSOR_PIN_2);
  int irSensor3Status = digitalRead(IR_SENSOR_PIN_3);
  int irSensor4Status = digitalRead(IR_SENSOR_PIN_4);
  int irSensor5Status = digitalRead(IR_SENSOR_PIN_5);
  int irSensor6Status = digitalRead(IR_SENSOR_PIN_6);

  // publish IR sensor status to MQTT topic
  char payload[50];
  sprintf(payload, "{\"ir_sensor_1\":%d,\"ir_sensor_2\":%d,\"ir_sensor_3\":%d,\"ir_sensor_4\":%d,\"ir_sensor_5\":%d,\"ir_sensor_6\":%d}",
          irSensor1Status, irSensor2Status, irSensor3Status, irSensor4Status, irSensor5Status, irSensor6Status);
  mqttClient.publish("ir_sensor_status_topic", payload);
  // loop to handle MQTT messages
  mqttClient.loop();
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Received message on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  Serial.println((char*)payload);

  // do something with the message here
}

void connectToMqttBroker() {
  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (mqttClient.connect("arduino_client")) {
      Serial.println("Connected to MQTT broker");
      mqttClient.subscribe("ir_sensor_status_topic");
    } else {
      Serial.print("Failed to connect to MQTT broker. Reason: ");
      Serial.println(mqttClient.state());
      delay(5000);
    }
}
