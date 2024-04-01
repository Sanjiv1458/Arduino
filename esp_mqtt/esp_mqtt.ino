/*
 Basic ESP8266 MQTT publish client example for a moisture sensor
*/
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "YOUR_SSID_NAME";
const char* password = "YOUR_PASSWORD";
const char* mqtt_server = "YOUR_NODE_RED_IP";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  // Connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  Serial.println("In reconnect...");
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("Arduino_Moisture")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}

void loop() {
  char msg[10];
  char msgtext[25];
  String themsg;
  if (!client.connected()) {
    reconnect();
  }
  
  int soil_moisture=analogRead(A0);  // read from analog pin A0
  Serial.print("analog value: ");
  Serial.println(soil_moisture);
  
  if((soil_moisture>300)&&(soil_moisture<700)) {
    Serial.println("Humid soil");
    sprintf(msgtext,"Humid soil",soil_moisture);
  } 
  else if ((soil_moisture>700)&&(soil_moisture<950)){
    Serial.println("Moist Soil");
    sprintf(msgtext,"Moist Soil",soil_moisture);
  }
  else if (soil_moisture <300){
    Serial.println("Needs water");    
    sprintf(msgtext,"Needs water",soil_moisture);
  }
  else
  {
      sprintf(msgtext,"Sensor Problem",soil_moisture);
  }

  sprintf(msg,"%i",soil_moisture);
  client.publish("humidity", msg);
  client.publish("soil", msgtext);
  delay(5000);
}
