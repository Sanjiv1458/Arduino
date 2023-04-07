int pirPin = 13;                  
int pirStat = 0;                   
 
void setup() {   
  pinMode(pirPin, INPUT);      
 
  Serial.begin(9600);
}
 
void loop(){
  
  pirStat = digitalRead(pirPin); 
   
  if (pirStat == HIGH) {            
    Serial.println("Hey I got you!!!");

  } 
  else {
    Serial.println("Not detected!!!");
   
  }
  delay(1000);
}
