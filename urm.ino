#include "Arduino.h"
#include "URM_UART.h"
#include <Servo.h> 

#define DefaultBaudrate 19200UL         // Urm06_UART
#define DefaultAddress 0x11   // Urm06_UART
Servo myservo;

unsigned long time;
URM_UART urm(Serial);
int distant = 0;
int count = 0;

int ENA = 10;
int INA1 = 9;
int INA2 = 8;
int cmd = 0;

long int t1;
long int t2;

void onRequestDistance(byte theAddress, int theDistance)
{
    Serial.print("Distance:");
    Serial.print(theDistance);
    Serial.println("mm\n\r");
}

void URM_06()
{
  if (urm.available())
      onRequestDistance(urm.receivedAddress, urm.receivedContent);
   static unsigned long sendingTimer=millis();
    if (millis()-sendingTimer>=1) {
      sendingTimer=millis();
      while(!urm.requestDistance(DefaultAddress)) {
          URM_06();    
        }}        
}

  
void wheel()
{
    digitalWrite(6,1);
    digitalWrite(ENA, HIGH);
    digitalWrite(INA1, HIGH);
    digitalWrite(INA2, LOW);
    delay(10000);
  
    //Serial.println("stop");
    digitalWrite(ENA, LOW);
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, LOW);
    delay(5000);
    
    //Serial.println("Backward");
    digitalWrite(ENA, HIGH);
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, HIGH);
    delay(10000);
    
   // Serial.println("stop");
    digitalWrite(ENA, LOW);
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, LOW);
    digitalWrite(6,0);
} 

void compare(int a){ 
    int i=0;
    t1=0;
    t2=0;

    
    t1 = millis();
    while ((urm.receivedContent >= a-5)&&(urm.receivedContent<= a+5)){ 
    for(i=0;i<120;i++){ 
    URM_06();
    myservo.write(i);
    delay(50);
    t2 = millis();

    if ((t2-t1)>=7000){
      wheel();
      break;
    }
    else if ((urm.receivedContent <= a-5)&&(urm.receivedContent>= a+5))
    break;
    }}  
}

void setup()
{
    urm.begin(DefaultBaudrate);
    myservo.attach(7); 
     
    pinMode(ENA, OUTPUT);
    pinMode(INA1, OUTPUT);
    pinMode(INA2, OUTPUT);
    pinMode(6, OUTPUT);
    digitalWrite(ENA, LOW);
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, LOW);

    URM_06();
}

void loop()
{
  int i=0;
  int distant=0;

  URM_06();
  URM_06();
  URM_06();
  distant=urm.receivedContent;
    Serial.print("00:");
    Serial.print(distant);
    Serial.println("mm\n\r");
  URM_06();
  URM_06();
  //delay(10);
  if((urm.receivedContent >= distant-5)&&(urm.receivedContent<= distant+5)){
    compare(distant);
    }
  else if(urm.receivedContent > 1000){
    for(i=0;i<120;i++){ 
      URM_06();
      myservo.write(i);
      delay(50);
      }}
  else 
    URM_06();}
