char t;

void setup() {
Serial.begin(9600);
pinMode(13,OUTPUT);   
pinMode(12,OUTPUT);   
pinMode(11,OUTPUT);  
pinMode(10,OUTPUT);   


}

void loop() {
if(Serial.available()){
  t = Serial.read();
  Serial.println(t);
}

if(t == 'F'){            
  digitalWrite(13,HIGH);
  digitalWrite(11,HIGH);
}

else if(t == 'B'){      
  digitalWrite(12,HIGH);
  digitalWrite(10,HIGH);
}

else if(t == 'L'){      
  digitalWrite(11,HIGH);
}

else if(t == 'R'){      
  digitalWrite(13,HIGH);
}

else if(t == 'S'){      
  digitalWrite(13,LOW);
  digitalWrite(12,LOW);
  digitalWrite(11,LOW);
  digitalWrite(10,LOW);
}
delay(100);
}
