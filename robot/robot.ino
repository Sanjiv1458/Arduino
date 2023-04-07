#include "FirebaseESP8266.h"
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Servo.h>

Servo servo;

const int dhtpin = 2;
const int trigPin = 4;
const int echoPin = 0;
const int servopin = 5;

#define SOUND_VELOCITY 0.034

#define DHTTYPE    DHT22
DHT dht(dhtpin, DHTTYPE);

#define FIREBASE_HOST "sanjiv-radar-8d286-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "0xcaaVwbVZMVxEPuDjUJSVwsXO0YRLUR7d3pL63l"
#define WIFI_SSID "IBN-B"
#define WIFI_PASSWORD "CUPunjab"

FirebaseData firebaseData;
FirebaseData sensorData;
FirebaseJson json;

void setup()
{
  Serial.begin(115200); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  dht.begin();

  servo.attach(servopin);
  servo.write(0);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
  if(Firebase.connect == true)
  {
    Serial.print("Wifi Connected");
  }
}


void loop() {
  for (int i = 0; i <= 180; i=i+10) {
    servo.write(i);
    Firebase.setFloat(firebaseData, "/distance", calculateDistance());
    Firebase.setFloat(firebaseData, "/angle", i);
    Firebase.setFloat(firebaseData, "/temp", dht.readTemperature());
    Firebase.setFloat(firebaseData, "/humidity", dht.readHumidity());
  }
  for (int i = 180; i >= 0; i=i-10) {
    servo.write(i);
    Firebase.setFloat(firebaseData, "/distance", calculateDistance());
    Firebase.setFloat(firebaseData, "/angle", i);
    Firebase.setFloat(firebaseData, "/temp", dht.readTemperature());
    Firebase.setFloat(firebaseData, "/humidity", dht.readHumidity());
  }
}


float calculateDistance(void) {
  long duration;
  float distanceCm;

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * SOUND_VELOCITY / 2;
  return distanceCm;
}
