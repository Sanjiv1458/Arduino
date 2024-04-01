#include <Servo.h>
#include <Stepper.h>

#define IN2 2
#define IN3 3     //ground

#define IN4 4
#define IN5 5    //top


#define IN8 8
#define IN9 9  //middle


int servo_pin = 7;
int claw_speed = 20;  // decrease the value to increase the claw speed
Servo servo;

void setup() {
  Serial.begin(115200); // Init Serial at 115200 Baud Rate.

  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);

  pinMode(IN4, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(IN8, OUTPUT);
  pinMode(IN9, OUTPUT);

  servo.attach(servo_pin);
  servo.write(0);
}

void loop() {
  run_motor_ground(1);
  delay()
  run_motor_ground(2);
  delay(3000);
  stop_ground_motor();
  run_motor_middle(1);
  delay(10000);
  stop_middle_motor();
  run_motor_top(1);
  delay(20000);
  stop_top_motor();
  run_motor_middle(2);
  delay(10000);
  stop_middle_motor();
  run_motor_ground(1);
  delay(3000);
  stop_ground_motor();
  grip(2, 0, 35);
  delay(1000);
  run_motor_ground(2);
  delay(3000);
  stop_ground_motor();
  run_motor_middle(1);
  delay(10000);
  stop_middle_motor();
  run_motor_top(2);
  delay(20000);
  stop_top_motor();
  run_motor_middle(2);
  delay(10000);
  stop_middle_motor();
  run_motor_ground(1);
  delay(3000);
  stop_ground_motor();
}


void stop_ground_motor()
{
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
}
void stop_top_motor()
{
  digitalWrite(IN4, LOW);
  digitalWrite(IN5, LOW);
}

void stop_middle_motor()
{
  digitalWrite(IN8, LOW);
  digitalWrite(IN9, LOW);
}

void run_motor_ground(int i)
{ // pass 1 for forward, 2 for backward
  Serial.println("Running Motor");
  if (i == 1)
  { // move forward
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
  }
  else if (i == 2)
  { // move backward
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
  }
}

void run_motor_top(int i)
{ // pass 1 for forward, 2 for backward
  Serial.println("Running Motor");
  if (i == 1)
  { // move forward
    digitalWrite(IN4, LOW);
    digitalWrite(IN5, HIGH);
  }
  else if (i == 2)
  { // move backwar
    digitalWrite(IN4, HIGH);
    digitalWrite(IN5, LOW);
  }
}

void run_motor_middle(int i)
{ // pass 1 for forward, 2 for backward
  Serial.println("Running Motor");
  if (i == 1)
  { // move forward
    digitalWrite(IN8, HIGH);
    digitalWrite(IN9, LOW);
  }
  else if (i == 2)
  { // move backward
    digitalWrite(IN8, LOW);
    digitalWrite(IN9, HIGH);
  }
}

void grip(int i, int servo_low, int servo_high)
{ // pass 1 to close grip, 2 to open grip
  Serial.println("Running Servo");
  if (i == 1)
  { // grip close
    for (int i = servo_low; i <= servo_high; i += 3)
    {
      servo.write(i);
      delay(claw_speed);
    }
  }
  else if (i == 2)
  { // grip open
    for (int i = servo_high; i >= servo_low; i -= 3)
    {
      servo.write(i);
      delay(claw_speed);
    }
  }
}
