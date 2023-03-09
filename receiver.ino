#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include <Servo.h>
#define PIN_ENA 6 
#define PIN_ENB 5 
#define PIN_IN1 8 
#define PIN_IN2 7 
#define PIN_IN3 4 
#define PIN_IN4 3 

RF24 radio(9, 10); 
byte pipeNo;
int mas1[13];
int mas2[13];

byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; 
byte speed=3;
byte turn=3;
bool engine=true;

void setup() {
  radio.begin();
  radio.setAutoAck(1);   
  radio.setRetries(0, 15);  
  radio.enableAckPayload();  
  radio.setPayloadSize(32);   
  radio.openReadingPipe(1, address[1]);    
  radio.setChannel(96); 
  radio.setPALevel (RF24_PA_MAX); 
  radio.setDataRate (RF24_250KBPS); 
  radio.powerUp(); 
  radio.startListening();  

  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

void loop() {
  while ( radio.available(&pipeNo)) {  
    radio.read( &mas1, sizeof(mas1) );        
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, LOW);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, LOW);
    engine=mas1[0];
    mas2[1] = map(mas1[1], -100, 100, -100, 100); 
    mas2[2] = map(mas1[2], -100, 100, -500, 500); 
    mas2[3] = map(mas1[3], -100, 100, -500, 500);
    mas2[4] = map(mas1[4], -100, 100, -500, 500);
    speed=mas1[11]*50;
    turn=mas1[12]*50;
    if ( engine == true) { 
      if (mas2[2]>20){  
      analogWrite(PIN_ENA, speed); 
      analogWrite(PIN_ENB, speed); 
      digitalWrite(PIN_IN1, HIGH);
      digitalWrite(PIN_IN2, LOW);
      digitalWrite(PIN_IN3, LOW);
      digitalWrite(PIN_IN4, HIGH); 
        }
      if (mas2[2]<-20){ 
      analogWrite(PIN_ENA, speed); 
      analogWrite(PIN_ENB, speed); 
      digitalWrite(PIN_IN1, LOW);
      digitalWrite(PIN_IN2, HIGH);
      digitalWrite(PIN_IN3, HIGH);
      digitalWrite(PIN_IN4, LOW); 
        }
       if (mas2[3]>30){
      analogWrite(PIN_ENA, turn); 
      analogWrite(PIN_ENB, turn); 
      digitalWrite(PIN_IN1, HIGH);
      digitalWrite(PIN_IN2, LOW);
      digitalWrite(PIN_IN3, HIGH);
      digitalWrite(PIN_IN4, LOW);
        }
      if (mas2[3]<-30){
      analogWrite(PIN_ENA, turn); 
      analogWrite(PIN_ENB, turn); 
      digitalWrite(PIN_IN1, LOW);
      digitalWrite(PIN_IN2, HIGH);
      digitalWrite(PIN_IN3, LOW);
      digitalWrite(PIN_IN4, HIGH);
        }
  }
    if ( engine == false) {
      if (mas2[4]>10){ 
      analogWrite(PIN_ENA, map(mas2[4], -100, 100, 0, 255)); 
      digitalWrite(PIN_IN1, HIGH);
      digitalWrite(PIN_IN2, LOW);
        }
      if (mas2[4]<-10){ 
      analogWrite(PIN_ENA, map(mas2[4], 100, -100, 0, 255)); 
      digitalWrite(PIN_IN1, LOW);
      digitalWrite(PIN_IN2, HIGH);
        }
      if (mas2[2]>10){ 
      analogWrite(PIN_ENB, map(mas2[2], -100, 100, 0, 255)); 
      digitalWrite(PIN_IN3, LOW);
      digitalWrite(PIN_IN4, HIGH); 
        }
      if (mas2[2]<-10){ 
      analogWrite(PIN_ENB, map(mas2[2], 100, -100, 0, 255)); 
      digitalWrite(PIN_IN3, HIGH);
      digitalWrite(PIN_IN4, LOW); 
        }    
    }
  }
}
