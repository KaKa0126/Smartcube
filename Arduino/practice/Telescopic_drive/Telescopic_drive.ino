/*
 냉장함 구동
 MOTOR1 = 5840_3650_40RPM
 MOTOR2 = JGB37_3525_45RPM
*/

#include <MsTimer2.h>  //실시간으로 스위치 상태 체크하기위한 라이브러리
#include <Wire.h>

#define ENA 3
#define ENB 9
#define IN1 24
#define IN2 25
#define IN3 26
#define IN4 27
#define PWM1 4
#define PWM2 10
#define relay1 30
#define relay2 31
#define FG1 5
#define FG2 11

int pos = 0;
int i = 0;
int a = 255;  //초기속도 가장 빠른 값
int currentspeed1 = 0; //현재속도
int currentspeed2 = 0; //현재속도
int switch1;  //Telescopic_drive_base에 부착되어, 함이 닫혔을 때, 눌린상태(0) 값 유지
int switch2;  //Cooling-cell-push-bar가 원점이면, 눌린상태(0) 값 유지

int MOTOR1testms = 20;
int MOTOR1testpersist = 1600;
int MOTOR1testdelay = 4000;
int MOTOR2testms = 20;          //수정
int MOTOR2testpersist = 2300;   //수정
int MOTOR2testdelay = 4000;     //수정

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);  
  pinMode(IN4, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(PWM2, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(FG1, INPUT);
  pinMode(FG2, INPUT);
  
  Serial.begin(9600);

  MsTimer2::set(20, switchcheck); //20ms마다 스위치 상태 측정하겠다.
  MsTimer2::start();

  pinMode(6, INPUT_PULLUP); //switch1;눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  pinMode(12, INPUT_PULLUP); //switch2;
  
  Serial.println("type B >>> MOTOR1 suddenCW");
  Serial.println("type N >>> MOTOR1 suddenCCW");
  Serial.println("type M >>> MOTOR1 suddenSTOP");
  Serial.println("type J >>> MOTOR2 suddenCW");
  Serial.println("type K >>> MOTOR2 suddenCCW");
  Serial.println("type L >>> MOTOR2 suddenSTOP");
  Serial.println("type Z >>> MOTOR1 CW");
  Serial.println("type X >>> MOTOR1 CCW");
  Serial.println("type C >>> MOTOR1 STOP");
  Serial.println("type A >>> MOTOR2 CW");
  Serial.println("type S >>> MOTOR2 CCW");
  Serial.println("type D >>> MOTOR2 STOP");
  Serial.println("type T >>> MOTOR1 TEST");
  Serial.println("type Y >>> MOTOR2 TEST");
  Serial.println("type R >>> switchread");
  Serial.println("type G >>> SUBTEST");
  Serial.println("type Q >>> MOTOR1OPEN");
  Serial.println("type W >>> MOTOR1CLOSE");
  Serial.println("type O >>> MOTOR2OPEN");
  Serial.println("type P >>> MOTOR2CLOSE");
}

void MOTOR1OPEN()
{
  MOTOR1CW();
  delay(MOTOR1testpersist);
  MOTOR1STOP();
}

void MOTOR1CLOSE()
{
  MOTOR1CCW();    
  //delay(MOTOR1testpersist+150);   Switch 없이 문 닫을 때 사용하는, 일정 시간 구동 코드
  while(switch1 == 1){
    Serial.print("");    
  }
  MOTOR1STOP();
}

void MOTOR2OPEN()
{
  MOTOR2CW();
  delay(MOTOR2testpersist);
  MOTOR2STOP();
}

void MOTOR2CLOSE()
{
  MOTOR2CCW();    
  //delay(MOTOR2testpersist+150);   Switch 없이 문 닫을 때 사용하는, 일정 시간 구동 코드
  while(switch2 == 1){
    Serial.print("");    
  }
  MOTOR2STOP();
}

void SUBTEST()
{
  int c = 5;
  while(c > 0){
    MOTOR2suddenCW();
    delay(500);
    MOTOR2suddenSTOP();
    delay(1000);
    MOTOR2suddenCCW();
    while(switch1 == 1){
      Serial.print("");    
    }
    MOTOR2suddenSTOP();
    delay(1000);
    c -= 1;
  }
}

void MOTOR1TEST()
{
  delay(5000);
  int cnt = 0;
  
  while(cnt < 1){
    cnt += 1;
    
    MOTOR1CW();
    delay(MOTOR1testpersist);
    MOTOR1STOP();
    delay(MOTOR1testdelay);
    MOTOR1CCW();
    
    delay(MOTOR1testpersist+150);
    /*
    while(switch1 == 1){
      Serial.print("");
    }
    */
    
    MOTOR1STOP();
    Serial.println(cnt);
    delay(MOTOR1testdelay);
  }
}

void MOTOR2TEST()
{
  int cnt = 0;
  while(cnt < 1){
    cnt += 1;
    
    MOTOR2CW();
    delay(MOTOR2testpersist);
    MOTOR2STOP();
    delay(MOTOR2testdelay);
    MOTOR2CCW();
    
    delay(MOTOR2testpersist);
    /*
    while(switch2 == 1){
      Serial.print("");
    }*/
    
    MOTOR2STOP();
    Serial.println(cnt);
    delay(MOTOR2testdelay);
  }
}

void switchcheck() {
  switch1 = digitalRead(6);
  switch2 = digitalRead(12);
}

void switchread() {
  Serial.print("switch1 = ");
  Serial.println(switch1);
  Serial.print("switch2 = ");
  Serial.println(switch2);
}

void MOTOR1suddenCW()
{  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, LOW);
  analogWrite(ENA, 255);
}

void MOTOR1suddenCCW()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, HIGH);
  analogWrite(ENA, 255);
}

void MOTOR1suddenSTOP()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void MOTOR2suddenCW()
{  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, LOW);
  analogWrite(ENB, 255);
}

void MOTOR2suddenCCW()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, HIGH);
  analogWrite(ENB, 255);
}

void MOTOR2suddenSTOP()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void MOTOR1CW()
{  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, LOW);
  for(i = 0 ; i <= a ; i=i+5){
    analogWrite(ENA, i);
    delay(MOTOR1testms);
  }
}

void MOTOR1CCW()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, HIGH);
  for(i = 0 ; i <= a ; i=i+5){
    analogWrite(ENA, i);
    delay(MOTOR1testms);
  }
}

void MOTOR1STOP()
{
  for(i = a ; i >= 0 ; i=i-5){
    analogWrite(ENA, i);
    delay(MOTOR1testms);
  }
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void MOTOR2CW()
{  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, LOW);
  for(i = 100 ; i <= a ; i=i+5){
    analogWrite(ENB, i);
    delay(MOTOR2testms);
  }
}

void MOTOR2CCW()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, HIGH);
  for(i = 100 ; i <= a ; i=i+5){
    analogWrite(ENB, i);
    delay(MOTOR2testms);
  }
}

void MOTOR2STOP()
{
  for(i = a ; i >= 0 ; i=i-5){
    analogWrite(ENB, i);
    delay(MOTOR2testms);
  }
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'B') {
      MOTOR1suddenCW();
    }
    else if (input == 'N') {
      MOTOR1suddenCCW();
    }
    else if (input == 'M') {
      MOTOR1suddenSTOP();
    }
    else if (input == 'J') {
      MOTOR2suddenCW();
    }
    else if (input == 'K') {
      MOTOR2suddenCCW();
    }
    else if (input == 'L') {
      MOTOR2suddenSTOP();
    }
    else if (input == 'Z') {
      MOTOR1CW();
    }
    else if (input == 'X') {
      MOTOR1CCW();
    }
    else if (input == 'C') {
      MOTOR1STOP();
    }
    else if (input == 'A') {
      MOTOR2CW();
    }
    else if (input == 'S') {
      MOTOR2CCW();
    }
    else if (input == 'D') {
      MOTOR2STOP();
    }
    else if (input == 'T') {
      MOTOR1TEST();
    }
    else if (input == 'Y') {
      MOTOR2TEST();
    }
    else if (input == 'R') {
      switchread();
    }
    else if (input == 'G') {
      SUBTEST();
    }
    else if (input == 'Q') {
      MOTOR1OPEN();
    }
    else if (input == 'W') {
      MOTOR1CLOSE();
    }
    else if (input == 'O') {
      MOTOR2OPEN();
    }
    else if (input == 'P') {
      MOTOR2CLOSE();
    }
  }
}
