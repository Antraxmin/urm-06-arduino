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
    static unsigned long sendingTimer = millis();
    if (millis() - sendingTimer >= 1) {
        sendingTimer=millis();
        while(!urm.requestDistance(DefaultAddress)) {
            URM_06();    
        }
    }        
}

  
void wheel()
{
    Serial.println("휠함수 호출");
    digitalWrite(22,1);
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
    digitalWrite(22,0);
} 

void compare(int a){ 
    int i=0;
    t1=0;
    t2=0;

    Serial.println(t2-t1);


    t1 = millis();
    bool check = true;
    while ((urm.receivedContent >= a-5)&&(urm.receivedContent<= a+5) && (check == true)) { 
      for(i = 0; i < 120; i++) {  // 오류 - 모터 두번 돌아가고 더이상 돌지 않음
        Serial.println("i값= ");
        Serial.println(i);
        URM_06(); // 여기까지만 실행됨 (계속 URM_06 실행 => wheel() 실행 반복) 
        myservo.write(i);
        delay(50);
        t2 = millis();

        Serial.println("t2-t1= ");
         Serial.println(t2-t1);

        if ((t2-t1) >= 7000){   // 7초가 지나면 동작
          Serial.println("compare함수 if문 실행");
          wheel();
          check=false;
          break;
        }
        
        else if ((urm.receivedContent <= a-5) && (urm.receivedContent >= a+5))
          break;
       }   
    }  
}

void setup()
{
    urm.begin(DefaultBaudrate);
    myservo.attach(7); 
     
    pinMode(ENA, OUTPUT);
    pinMode(INA1, OUTPUT);
    pinMode(INA2, OUTPUT);
    pinMode(22, OUTPUT);
    digitalWrite(ENA, LOW);
    digitalWrite(INA1, LOW);
    digitalWrite(INA2, LOW);
    digitalWrite(22,0);

    URM_06();
}

void loop()
{
  int i = 0;
  int distant = 0;

  //URM_06();
  //URM_06();
  //URM_06();
  distant=urm.receivedContent;
    Serial.print("00:");
    Serial.print(distant);
    Serial.println("mm\n\r");
  //URM_06();
  //URM_06();
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
    URM_06();
}
