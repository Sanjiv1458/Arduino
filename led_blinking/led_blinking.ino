// Define the pin for the IR sensor
const int IR_PIN = 2;

void setup() {
  Serial.begin(9600); // Initialize serial communication
  pinMode(IR_PIN, INPUT); // Set the IR pin as an input
}

void loop() {
  int IR_value = digitalRead(IR_PIN); // Read the value from the IR sensor
  Serial.println(IR_value); // Print the value to the serial monitor
  delay(100); // Delay for 100 milliseconds
}
