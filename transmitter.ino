#include <SPI.h>           
#include "nRF24L01.h"         
#include "RF24.h"             
#include <Adafruit_GFX.h>     
#include <Adafruit_SSD1306.h> 
#define OLED_RESET 7          


RF24 radio(9, 10);
Adafruit_SSD1306 display(OLED_RESET); 

int mas1[13];
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"}; 
byte turn=3;
byte speed=3;
byte state=1;
bool engine=false;
bool update=true;

bool b1,b2,b3,b4,b5,b6=0;
bool flag1,flag2,flag3,flag4,flag5,flag6=false;
long t1,t2,t3,t4,t5,t6;
  
void setup() {
  pinMode(2, INPUT_PULLUP); 
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP); 
  pinMode(5, INPUT_PULLUP);  
  pinMode(7, INPUT_PULLUP); 
  pinMode(8, INPUT_PULLUP);

  pinMode(A2, INPUT); 
  pinMode(A3, INPUT);
  pinMode(A0, INPUT); 
  pinMode(A1, INPUT);
  
  radio.begin();
  radio.setAutoAck(1);     
  radio.setRetries(0, 15); 
  radio.enableAckPayload();  
  radio.setPayloadSize(32);    
  radio.openWritingPipe(address[1]);  
  radio.setChannel(96);  
  radio.setPALevel (RF24_PA_MAX); 
  radio.setDataRate (RF24_250KBPS); 
  radio.powerUp(); 
  radio.stopListening();  

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  
  display.clearDisplay(); 
  display.setTextSize(2); 
  display.setTextColor(WHITE); 
  display.setCursor(0, 0); 
  display.print ("Loading.."); 
  display.display(); 
  delay(1000);
}

void loop() {  
  mas1[5] = digitalRead(8); 
  mas1[6] = digitalRead(7);

  mas1[7] = digitalRead(2); 
  mas1[8] = digitalRead(3);

  mas1[9] = digitalRead(4); 
  mas1[10] = digitalRead(5); 

  mas1[1]= map(analogRead(A2),0,1023,-100,100); 
  mas1[2]= map(analogRead(A3),0,1023,100,-100); 
  mas1[3]= map(analogRead(A0),0,1023,-100,100); 
  mas1[4]= map(analogRead(A1),0,1023,100,-100); 

if (mas1[1]<10&&mas1[1]>-10)mas1[1]=0;  
if (mas1[2]<10&&mas1[2]>-10)mas1[2]=0;
if (mas1[3]<10&&mas1[3]>-10)mas1[3]=0;
if (mas1[4]<10&&mas1[4]>-10)mas1[4]=0;

if (mas1[1]<-100)mas1[1]=-100; 
if (mas1[2]<-100)mas1[2]=-100;
if (mas1[3]<-100)mas1[3]=-100;
if (mas1[4]<-100)mas1[4]=-100;

if (mas1[1]>100)mas1[1]=100;  
if (mas1[2]>100)mas1[2]=100;
if (mas1[3]>100)mas1[3]=100;
if (mas1[4]>100)mas1[4]=100;

if (b1 && !flag1 && millis() - t1 > 100) {flag1 = true;t1 = millis();
update = true;
engine=!engine;

}if (!b1 && flag1 && millis() - t1 > 100) {flag1 = false;t1 = millis();}b1 = digitalRead(8);

if (b2 && !flag2 && millis() - t2 > 100) {flag2 = true;t2 = millis();
if(state==1){
  display.clearDisplay(); 
  display.display();
}

}if (!b2 && flag2 && millis() - t2 > 100) {flag2 = false;t2 = millis();}b2 = digitalRead(7);

if (b3 && !flag3 && millis() - t3 > 100) {flag3 = true;t3 = millis();
update = true;
state++;
}if (!b3 && flag3 && millis() - t3 > 100) {flag3 = false;t3 = millis();}b3 = digitalRead(2);

if (b4 && !flag4 && millis() - t4 > 100) {flag4 = true;t4 = millis();
update = true;
state--;
}if (!b4 && flag4 && millis() - t4 > 100) {flag4 = false;t4 = millis();}b4 = digitalRead(3);

if (b5 && !flag5 && millis() - t5 > 100) {flag5 = true;t5 = millis();
update = true;
if (state==2){
 speed--; 
}
if (state==3){
 turn--; 
}
}if (!b5 && flag5 && millis() - t5 > 100) {flag5 = false;t5 = millis();}b5 = digitalRead(4);

if (b6 && !flag6 && millis() - t6 > 100) {flag6 = true;t6 = millis();
update = true;
if (state==2){
 speed++; 
}
if (state==3){
 turn++; 
}
}if (!b6 && flag6 && millis() - t6 > 100) {flag6 = false;t6 = millis();}b6 = digitalRead(5);

if(turn>5)turn=5;
if(turn<1)turn=1;
if(state>3)state=1;
if(state<1)state=3;
if(speed>5)speed=5;
if(speed<1)speed=1;

mas1[0]=engine;
mas1[11]=speed;
mas1[12]=turn;

if (state==1){ 
if (update == true){
  display.clearDisplay(); 
  display.setCursor(10, 0);
  display.setTextColor(WHITE);
  if(engine==true){
  display.print ("[L]---[R]");
  }
  else {
  display.print ("[L]   [R]");
  }
  display.setCursor(16, 16);
  display.print (speed);
  display.setCursor(100, 16);
  display.print (turn);
  display.display(); 
  update = false;
}
}


if (state==2){ 
if (update == true){
  display.clearDisplay(); 
  display.setCursor(10, 0);
  display.setTextColor(WHITE);
  display.fillRoundRect(-10, 15, 54, 16, 4, WHITE); 
  if(engine==true){
  display.print ("[L]---[R]");
  }
  else {
  display.print ("[L]   [R]");
  }
  display.setTextColor(BLACK);
  display.setCursor(16, 16);
  display.print (speed);
  display.setTextColor(WHITE);
  display.setCursor(100, 16);
  display.print (turn);
  display.display(); 
  update = false;
}}

if (state==3){ 
if (update == true){
  display.clearDisplay(); 
  display.setCursor(10, 0);
  display.setTextColor(WHITE);
  display.fillRoundRect(84, 15, 54, 16, 4, WHITE); 
  if(engine==true){
  display.print ("[L]---[R]");
  }
  else {
  display.print ("[L]   [R]");
  }
  display.setCursor(16, 16);
  display.print (speed);
  display.setTextColor(BLACK);
  display.setCursor(100, 16);
  display.print (turn);
  display.display(); 
  update = false;
}}
  radio.write(&mas1, sizeof(mas1)); 
}
