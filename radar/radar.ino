// for more project visit www.blackkeyhole.com
// crafted by Ashshak Sharifdeen

// Radar
#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <Servo.h>

// DHT
#include "DHT.h"
#include "DHT_U.h"
#define DHTTYPE DHT11     // type of the temperature sensor
const int DHTPin = 5;     //--> The pin used for the DHT11 sensor is Pin D1 = GPIO5
DHT dht(DHTPin, DHTTYPE); //--> Initialize DHT sensor, DHT dht(Pin_used, Type_of_DHT_Sensor);

WiFiClientSecure client; //--> Create a WiFiClientSecure object.

const int trigPin = 2;
const int echoPin = 4;

// Defines piezo pin
const int piezoPin = D0;
Servo myServo;
int notes[] = {262, 462, 862, 1662, 3262}; // Enter here the notes you like

// Variables for the duration and the distance
long duration;
float distance;

float a;
float d;
String sheet_range = "";
String sheet_Distance = "";

const char *ssid = "IBN-B";        // replace with our wifi ssid
const char *password = "CUPunjab"; // replace with your wifi password

const char *host = "script.google.com";
String GAS_ID = "AKfycbyGn5uKXfLzKFVU9D_JoKnc_XJNM1x07Q4Ze7B7NAzUTMU8pMx51aLAc3yxx4RlC9kuTg"; // Replace with your own google script id

const int httpsPort = 443;                                                                         // the https port is same

// echo | openssl s_client -connect script.google.com:443 |& openssl x509 -fingerprint -noout

void setup()
{

  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT);  // Sets the echoPin as an Input

  myServo.attach(14);

  Serial.begin(115200);

  delay(500);
  dht.begin(); //--> Start reading DHT11 sensors
  delay(500);

  Serial.println();
  Serial.print("Connecting to wifi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  Serial.print("Connecting");

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  client.setInsecure();
  Serial.print("Connecting to ");
  Serial.println(host); // try to connect with "script.google.com"

  // Try to connect for a maximum of 5 times then exit
  bool flag = false;
  for (int i = 0; i < 5; i++)
  {
    int retval = client.connect(host, httpsPort);
    if (retval == 1)
    {
      flag = true;
      break;
    }
    else
      Serial.println("Connection failed. Retrying...");
  }

  if (!flag)
  {
    Serial.print("Could not connect to server: ");
    Serial.println(host);
    Serial.println("Exiting...");
    return;
  }
  // Finish setup() function in 1s since it will fire watchdog timer and will reset the chip.
  // So avoid too many requests in setup()

  Serial.println("\nWrite into cell 'A1'");
  Serial.println("------>");

  Serial.println("\nGET: Fetch Google Calendar Data:");
  Serial.println("------>");
  Serial.println("\nStart Sending Sensor Data to Google Spreadsheet");
}

int calculateDistance()
{

  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH); // Reads the echoPin, returns the sound wave travel time in microseconds
  // U(m/s)=dX(m)/dT(s)
  // in this case Duration(time)= 2*Distance/SpeedOfSound=>
  // Distance=SpeedOfSound*Duration/2
  //  In dry air at 20 °C, the speed of sound is 343.2 m/s or 0.003432 m/Microsecond or 0,03434 cm/Microseconds
  distance = duration * 0.034 / 2;
  return distance;
}

void sendData(float angle, float dist, int hum, float tem)
{
  Serial.println("==========");
  Serial.print("connecting to ");
  Serial.println(host);

  //----------------------------------------Connect to Google host
  if (!client.connect(host, httpsPort))
  {
    Serial.println("connection failed");
    return;
  }
  //----------------------------------------

  //----------------------------------------Processing data and sending data
  sheet_range = String(angle);
  sheet_Distance = String(dist);
  String string_temperature = String(tem);
  String string_humidity = String(hum, DEC);
  String url = "/macros/s/" + GAS_ID + "/exec?sheetrange=" + sheet_range + "&distance=" + sheet_Distance + "&humidity" + string_temperature + "&temperature=" + string_temperature;
  Serial.print("requesting URL: ");
  Serial.println(url);

  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: BuildFailureDetectorESP8266\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent");
  //----------------------------------------

  //----------------------------------------Checking whether the data was sent successfully or not
  while (client.connected())
  {
    String line = client.readStringUntil('\n');
    if (line == "\r")
    {
      Serial.println("headers received");
      break;
    }
  }
  String line = client.readStringUntil('\n');
  if (line.startsWith("{\"state\":\"success\""))
  {
    Serial.println("esp8266/Arduino CI successfull!");
  }
  else
  {
    Serial.println("esp8266/Arduino CI has failed");
  }
  Serial.print("reply was : ");
  Serial.println(line);
  Serial.println("closing connection");
  Serial.println("==========");
  Serial.println();
  //----------------------------------------
}

void loop()
{

  for (a = 15; a <= 165; a++)
  {
    myServo.write(a);

    distance = calculateDistance(); // Calls a function for calculating the distance measured by the Ultrasonic sensor for each degree

    // beep sequence
    if (distance > 40)
    {
      noTone(piezoPin);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }
    else if (distance <= 40 && distance > 30)
    {
      tone(piezoPin, notes[1]);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }
    else if (distance <= 30 && distance > 20)
    {
      tone(piezoPin, notes[2]);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }
    else if (distance <= 20 && distance > 10)
    {
      tone(piezoPin, notes[3]);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }
    else
    {
      tone(piezoPin, notes[4]);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    int h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
      Serial.println("Failed to read from DHT sensor !");
      delay(500);
      return;
    }

    String Temp = "Temperature : " + String(t) + " °C";
    String Humi = "Humidity : " + String(h) + " %";
    String sheetRange = "Angle: " + String(a) + String("°");
    String sheetDistance = "Distance: " + String(distance) + String("cm");

    Serial.print(sheetRange);
    Serial.print(",");
    Serial.print(sheetDistance);
    Serial.print(",");
    Serial.println(Temp);
    Serial.print(",");
    Serial.println(Humi);
    Serial.print(".");

    Serial.println("POST or SEND Sensor data to Google Spreadsheet:"); // switch to add
    sendData(a, distance, h, t);
  }
  // Repeats the previous lines from 165 to 15 degrees
  for (float a = 165; a > 15; a--)
  {
    myServo.write(a);

    distance = calculateDistance();
    if (distance > 40)
    {
      noTone(piezoPin);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }
    else if (distance <= 40 && distance > 30)
    {
      tone(piezoPin, notes[1]);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }
    else if (distance <= 30 && distance > 20)
    {
      tone(piezoPin, notes[2]);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }
    else if (distance <= 20 && distance > 10)
    {
      tone(piezoPin, notes[3]);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }
    else
    {
      tone(piezoPin, notes[4]);
      delay(10);
      noTone(piezoPin);
      delay(30);
    }

    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    int h = dht.readHumidity();
    // Read temperature as Celsius (the default)
    float t = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(h) || isnan(t))
    {
      Serial.println("Failed to read from DHT sensor !");
      delay(500);
      return;
    }

    String Temp = "Temperature : " + String(t) + " °C";
    String Humi = "Humidity : " + String(h) + " %";
    String sheetRange = "Angle: " + String(a) + String("°");
    String sheetDistance = "Distance: " + String(distance) + String("cm");

    Serial.print(sheetRange);
    Serial.print(",");
    Serial.print(sheetDistance);
    Serial.print(",");
    Serial.println(Temp);
    Serial.print(",");
    Serial.println(Humi);
    Serial.print(".");

    sendData(a, distance, h, t);
  }
}
