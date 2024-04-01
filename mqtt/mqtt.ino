#include <WiFiNINA.h>
#include <PubSubClient.h>
#include <DHT.h>

#define LED_PIN 5
#define DHTPIN 2
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

const char* ssid = "your-ssid";
const char* password = "your-password";
const char* mqtt_server = "mqtt://broker.hivemq.com:1883";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

void setup() {
  Serial.begin(9600);
  dht.begin();

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  mqttClient.setServer(mqtt_server, 1883);

  while (!mqttClient.connected()) {
    Serial.println("Connecting to MQTT broker...");
    if (mqttClient.connect("arduino-client")) {
      Serial.println("Connected to MQTT broker");
      mqttClient.subscribe("led");
    }
    else {
      Serial.print("Failed to connect to MQTT broker, rc=");
      Serial.println(mqttClient.state());
      delay(5000);
    }
  }
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read temperature and humidity from DHT sensor");
    delay(2000);
    return;
  }

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print(" °C, Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");

  char temperatureString[8];
  dtostrf(temperature, 6, 2, temperatureString);

  char humidityString[8];
  dtostrf(humidity, 6, 2, humidityString);

  mqttClient.publish("temperature", temperatureString);
  mqttClient.publish("humidity", humidityString);

  mqttClient.loop();

  if (mqttClient.connected()) {
    if (mqttClient.subscribe("led")) {
      if (mqttClient.available()) {
        String message = mqttClient.readString();
        if (message == "on") {
          digitalWrite(LED_PIN, HIGH);
        }
        else if (message == "off") {
          digitalWrite(LED_PIN, LOW);
        }
      }
    }
  }
  delay(5000);
}
