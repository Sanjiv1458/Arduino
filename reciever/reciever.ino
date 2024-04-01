///////Reciever////////

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>


RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

char t;
 
void setup() {
radio.begin();
radio.openWritingPipe(address);
radio.setPALevel(RF24_PA_MIN);
radio.startListening();
pinMode(6,OUTPUT);
pinMode(5,OUTPUT);
pinMode(4,OUTPUT);
pinMode(3,OUTPUT);
} 
void loop() {
if(radio.available()) {
  radio.read(&t, sizeof(t));
if(t == 'F'){           
  digitalWrite(6,HIGH);
  digitalWrite(4,HIGH);
}
else if(t == 'B'){      
  digitalWrite(5,HIGH);
  digitalWrite(3,HIGH);
}
 
else if(t == 'L'){      
  digitalWrite(4,HIGH);
}
 
else if(t == 'R'){     
  digitalWrite(6,HIGH);
}
else if(t == 'S'){
  digitalWrite(6,LOW);
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);
  digitalWrite(3,LOW);
}
}
delay(100);
}
