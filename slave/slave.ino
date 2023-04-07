char t;
 
void setup() {
pinMode(10,OUTPUT);   
pinMode(11,OUTPUT);  
pinMode(12,OUTPUT); 
pinMode(13,OUTPUT);   
Serial.begin(9600);
} 

void loop() {
 if(Serial.available() > 0){ 
    t = Serial.read(); 
 }
 
  if(t == 'R')
  {
  analogWrite(12,150);
  delay(800);
  digitalWrite(12,LOW);
  delay(100);
  }
  else if(t == 'L')
  {
  analogWrite(13,150);
  delay(800);
  digitalWrite(13,LOW);
  delay(100);
  }
  else if(t == 'J')
  {
  analogWrite(10,150);
  delay(800);
  digitalWrite(10,LOW);
  delay(100);
  }
  else if(t == 'K')
  {
  analogWrite(11,150);
  delay(800);
  digitalWrite(11,LOW);
  delay(100);
  }
  else if(t == 'F')
  {
  analogWrite(12, 150);
  analogWrite(10, 150);
  delay(1000);
  digitalWrite(12, LOW);
  digitalWrite(10, LOW);
  delay(100);
  }
  else if(t == 'B')
  {
  analogWrite(13, 150);
  analogWrite(11, 150);
  delay(1000);
  digitalWrite(13, LOW);
  digitalWrite(11, LOW);
  }
  delay(100);
}
