int sensor = A0; 
int val = 0;  
int limit = 300; 

void setup() {
 Serial.begin(9600);
 pinMode(13, OUTPUT);
}

void loop() {

 val = analogRead(sensor); 
 Serial.println("Analog Value : ");
 Serial.println(val);
 
 if (val<limit) {
 digitalWrite(13, HIGH); 
 }
 else {
 digitalWrite(13, LOW); 
 }
 
 delay(1000); 
}
