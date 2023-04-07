void setup() {
pinMode(10,OUTPUT);   
pinMode(11,OUTPUT);  
pinMode(12,OUTPUT); 
pinMode(13,OUTPUT);   
Serial.begin(9600);
}

void loop() {

  Serial.println("R");
  analogWrite(12,150);
  delay(800);
  digitalWrite(12,LOW);
  delay(100);
  

  Serial.println("L");
  analogWrite(13,150);
  delay(800);
  digitalWrite(13,LOW);
  delay(100);


  Serial.println("J");
  analogWrite(10,150);
  delay(800);
  digitalWrite(10,LOW);
  delay(100);

 
  Serial.println("K");
  analogWrite(11,150);
  delay(800);
  digitalWrite(11,LOW);
  delay(100);

  Serial.println("F");
  analogWrite(12, 150);
  analogWrite(10, 100);
  delay(1000);
  digitalWrite(12, LOW);
  digitalWrite(10, LOW);
  delay(100);

  Serial.println("B");
  analogWrite(13, 150);
  analogWrite(11, 150);
  delay(1000);
  digitalWrite(13, LOW);
  digitalWrite(11, LOW);
  delay(100);
}
