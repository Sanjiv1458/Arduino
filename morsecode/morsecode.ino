char string[] = "DEEPSUNDAR";

void dot() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for 1 second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}
void dash() {
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(3000);                       // wait for 3 second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);
}

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  for (int i = 0; i < sizeof(string); i++) {
    if (string[i] == 'A') {
      dot();
      dash();
    }
    else if (string[i] == 'D') {
      dash();
      dot();
      dot();
    }
    else if (string[i] == 'E') {
      dot();
    }
    else if (string[i] == 'N') {
      dash();
      dot();
    }
    else if (string[i] == 'P') {
      dot();
      dash();
      dash();
      dot();
    }
    else if (string[i] == 'R') {
      dot();
      dash();
      dot();
    }
    else if (string[i] == 'S') {
      dot();
      dot();
      dot();
    }
    else if (string[i] == 'U') {
      dot();
      dot();
      dash();
    }
  }
}
