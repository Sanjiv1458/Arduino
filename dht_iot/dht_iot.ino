/included dht library
#include <DHT.h>;

//defined pin for led
#define DHTPIN 5

//defined the type of dht sensor
#define DHTTYPE DHT22

//creating a object of dht class
DHT dht22(DHTPIN,DHTTYPE);

//including the library
#include "thingProperties.h"

void setup() {
  
  //set the out-put pin
   pinMode(8, OUTPUT);

  // initialise the dht sensor
    dht22.begin();

  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500); 

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);
  

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}

void loop() {
  ArduinoCloud.update();
  //code here 
  
   onLedChange();
   //reading the humidity
    humidity = dht22.readHumidity();
    //reding the temperature
    temperature= dht22.readTemperature();
    //printing the temperature and humidity
    Serial.println(temperature);
    Serial.println(humidity);
    delay(1000);
}


void onLedChange()  {
  digitalWrite(8, led);
}
