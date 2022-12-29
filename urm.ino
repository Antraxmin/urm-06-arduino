#include "Arduino.h"
#include "URM_UART.h"
#include <Servo.h> 

#define DefaultBaudrate 19200UL         // Urm06_UART
#define DefaultAddress 0x11   // Urm06_UART
Servo myservo;

unsigned long time;
URM_UART urm(Serial);
int distant;
int count = 0;

int ENA = 10;
int INA1 = 9;
int INA2 = 8;
int cmd = 0;

long int t1;
long int t2;

bool checking = true;
int theDistance;

void onRequestDistance(byte theAddress, int theDistance)
{
    Serial.print("Distance값 = ");
    Serial.println(theDistance);
    //Serial.println("mm\n\r")
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
    //Serial.print("urm.receivedContent값= ");
    //Serial.println(urm.receivedContent);
}
  
void wheel()
{
    Serial.println("휠함수 호출");
    Serial.write(1);
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
    Serial.write(0);
} 

void compare(int a){ 
    int i=0;
    t1=0;
    t2=0;

    //Serial.println(t2-t1);
    
    t1 = millis();
    bool check = true;
    while ((urm.receivedContent >= a-5)&&(urm.receivedContent<= a+5) && (check == true)) { 
      Serial.print("a값= ");
      Serial.println(a);
      for(i = 0; i < 120; i++) { 
        //Serial.print("i값= ");
        //Serial.println(i);
        URM_06(); 
        myservo.write(i);
        delay(50);
        t2 = millis();

        Serial.print("t2-t1= ");
         Serial.println(t2-t1);

        if ((t2-t1) >= 7000){   // 7초가 지나면 동작
          wheel();
          check=false;
          break;
        }
        
        else if ((urm.receivedContent <= a-5) && (urm.receivedContent >= a+5)) {
          break;
          check=false;
        }
      }
      Serial.println("for문 종료");
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

    Serial.println("urm_06 함수 실행");
    URM_06();
    //URM_06();
    Serial.print("Distance값 = ");
    Serial.println(theDistance);
    Serial.println("mm\n\r");

    Serial.print("urm.receivedContent값= "); // 0 출력 
    Serial.println(urm.receivedContent);

    if((urm.receivedContent >= theDistance-5) && (urm.receivedContent <= theDistance+5)){
      Serial.println("1번 조건문 실행");
      compare(theDistance); 
    }

    else if(urm.receivedContent > 1000) {
      Serial.println("2번 조건문 실행");
      for(int i = 0; i < 120; i++){ 
        Serial.print("i값 = ");
        Serial.println(i);
        myservo.write(i);
        delay(50);
      }
      //URM_06();
    }

    // 3번 조건 -  장애물 측정 거리가 1000 (1m) 이하일때 서보모터 멈추고 1000이하인 곳만 계속해서 초음파 거리 측정
    else if (urm.receivedContent <= 1000){
      Serial.println("조건3실행");
      Serial.print("조건 3 urm.receivedContent값= "); // 0 출력 
      Serial.println(urm.receivedContent);
      Serial.println("조건3종료");
    }
    Serial.println("루프 종료");


  //theDistance = 0;
  //urm.receivedContent = 0;
}
