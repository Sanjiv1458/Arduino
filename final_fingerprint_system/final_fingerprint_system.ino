void setup() {
  // Set the built-in LED pin (pin 13) as an output
  pinMode(13, OUTPUT);
}

void loop() {
  // Turn off the built-in LED
  digitalWrite(13, LOW);
  delay(1000); // Wait for 1 second

  // Turn on the built-in LED
  digitalWrite(13, HIGH);
  delay(1000); // Wait for 1 second
}
