int led = 13;           
int bright = 0;   
int fadeamt = 5;    

void setup() {
  pinMode(led, OUTPUT);
}

void loop() {
  analogWrite(led, bright);

  bright = bright + fadeamt;

  if (bright <= 0 || bright >= 255) {
    fadeamt = -fadeamt;
  }
  delay(30);
}
