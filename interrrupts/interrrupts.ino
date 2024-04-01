int interruptPin1 = 2;
int interruptPin2 = 3;
int ledPin = 5;

void blink_LED_ON() {
  digitalWrite(ledPin, HIGH);
  Serial.write("ON");
}
void blink_LED_OFF() {
  digitalWrite(ledPin, LOW);
  Serial.write("OFF");
}

void setup() {
  Serial.begin(9600);
  pinMode(5, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(interruptPin1), blink_LED_ON, RISING);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), blink_LED_OFF, FALLING);
}

void loop() {
}
