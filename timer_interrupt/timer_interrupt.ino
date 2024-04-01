#include <TimerOne.h>

void setup()
{
  pinMode(13, OUTPUT));
  Timer1.intialize(100000);//set a timer of length 0.1 second or 10Hz => the led will blink 5 times on/off per second
  Timer1.attachInterrupt(timeIsr);
}

void loop()
{

}
void timeIsr()
{
  //Toggle LED
  digitalWrite(13, digitalRead(13) ^ 1); //OR digitalWrite(13, !digitalRead(13));
}



void setup() {
  // Set up Timer1 to interrupt every 1 millisecond
  cli(); // Disable global interrupts
  TCCR1A = 0; // Set Timer1 control registers to default
  TCCR1B = 0;
  TCNT1 = 0; // Initialize counter value to 0
  OCR1A = 15999; // Set compare match register to desired interval (1ms)
  TCCR1B |= (1 << WGM12); // Enable CTC mode
  TCCR1B |= (1 << CS11); // Set prescaler to 8 and start the timer
  TIMSK1 |= (1 << OCIE1A); // Enable Timer1 compare interrupt
  sei(); // Enable global interrupts
}

void loop() {
  // Your main code here
}

// Interrupt Service Routine (ISR) for Timer1 compare match
ISR(TIMER1_COMPA_vect) {
  interruptServiceRoutine();
}

// Function to be executed on each interrupt
void interruptServiceRoutine() {
  // Your code here
}
