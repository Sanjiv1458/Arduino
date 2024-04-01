// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(13, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(6, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(13, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       
  digitalWrite(13, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);

  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       
  digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  delay(100);// wait for a second
  
  
  digitalWrite(8, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       
  digitalWrite(8, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);// wait for a second

  digitalWrite(6, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       
  digitalWrite(6, LOW);    // turn the LED off by making the voltage LOW
  delay(100);                      
}
