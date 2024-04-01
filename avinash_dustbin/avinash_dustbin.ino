#include <TM1637Display.h>
#include <SoftwareSerial.h>
#include <Servo.h>

#define GSM_TX 7       // GSM module's TX pin connected to Arduino's digital pin 7
#define GSM_RX 8       // GSM module's RX pin connected to Arduino's digital pin 8
#define TRIGGER_PIN 5  // Ultrasonic sensor's trigger pin connected to Arduino's digital pin 5
#define ECHO_PIN 6     // Ultrasonic sensor's echo pin connected to Arduino's digital pin 6

#define PIR_PIN 9

#define SERVO_PIN 4  // Servo motor's signal pin connected to Arduino's digital pin 4
#define CLK 2        // define CLK pin to Arduino pin 2
#define DIO 3        // define DIO pin to Arduino pin 3

TM1637Display display(CLK, DIO);           // Create a TM1637Display object for the TM1637 display
SoftwareSerial gsmSerial(GSM_TX, GSM_RX);  // Create a software serial object for the GSM module
Servo servo;                               // Create a Servo object for the servo motor

void setup() {
  Serial.begin(9600);            // Initialize the serial port
  display.setBrightness(7);      // Set the display brightness (0-7)
  gsmSerial.begin(9600);         // Initialize the software serial port for the GSM module
  servo.attach(SERVO_PIN);       // Attach the servo motor to the signal pin
  pinMode(TRIGGER_PIN, OUTPUT);  // Set the trigger pin as an output
  pinMode(ECHO_PIN, INPUT);      // Set the echo pin as an input
  pinMode(PIR_PIN, INPUT);       // Declare PIR pin as input

  servo.write(0);
}

void loop() {
  // Measure the distance using the ultrasonic sensor
  digitalWrite(TRIGGER_PIN, LOW);  // Set the trigger pin to low for 2 microseconds
  delayMicroseconds(2);
  digitalWrite(TRIGGER_PIN, HIGH);  // Set the trigger pin to high for 10 microseconds
  delayMicroseconds(10);
  digitalWrite(TRIGGER_PIN, LOW);  // Set the trigger pin to low

  long duration = pulseIn(ECHO_PIN, HIGH);  // Measure the duration of the echo pulse
  float distance = duration * 0.034 / 2;    // Convert the duration to distance in centimeters

  // Calculate the filled percentage
  int filledPercentage =   100 - (int)(distance / 35 * 100);
  // Print the distance to the serial monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  // Display the filled percentage on the TM1637 display
  int digits[4];                                                // Array to hold the digits to be displayed
  digits[0] = filledPercentage / 1000;                          // Thousands digit
  digits[1] = (filledPercentage / 100) % 10;                    // Hundreds digit
  digits[2] = (filledPercentage / 10) % 10;                     // Tens digit
  digits[3] = filledPercentage % 10;                            // Ones digit
  display.showNumberDecEx(filledPercentage, 0b01000000, true);  // Display the number with leading zeros and a decimal point
  // Print the filled percentage to the serial monitor
  Serial.print("Filled percentage: ");
  Serial.print(filledPercentage);
  Serial.println("%");

  int minThreshold = 85;  // Set a threshold value to determine if the dustbin is full
  int fullThreshold = 100;  // Set a threshold value to determine if the dustbin is full

  Serial.print("PIR Status:");
  Serial.print(digitalRead(PIR_PIN));
  Serial.println(" ");
  // Check the PIR sensor to see if someone is nearby
  if (digitalRead(PIR_PIN) == HIGH) {
    // If someone is nearby and the dustbin is full, open the cap
    if (filledPercentage <= fullThreshold && filledPercentage >= minThreshold) {
      sendMessage();
      servo.write(90);  // Open the cap
      delay(5000);      // Keep the cap open for 5 seconds
      servo.write(0);   // Close the cap
    } else {
      servo.write(90);  // Open the cap
      delay(3000);      // Keep the cap open for 5 seconds
      servo.write(0);   // Close the cap
    }
  }
  delay(1000);  // Wait for 1 second before taking another measurement
}

void sendMessage() {
  // Send a message if the dustbin is filled
  gsmSerial.println("AT+CMGF=1");  // Set the GSM module to text mode
  delay(100);
  gsmSerial.println("AT+CMGS=\"+919815930069\"");  // Replace with your phone number
  delay(100);
  gsmSerial.print("The dustbin is full!");  // Send the message
  delay(100);
  gsmSerial.println((char)26);
}

void makeCall() {
  // Make a call if the dustbin is filled
  gsmSerial.println("ATD+919815930069;");  // Replace with the phone number to call
  delay(5000);  // Wait for 5 seconds (adjust as needed)
  gsmSerial.println("ATH");  // Hang up the call
  delay(1000);  // Wait for 1 second (adjust as needed)
}