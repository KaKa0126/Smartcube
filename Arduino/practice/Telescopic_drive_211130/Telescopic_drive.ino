/*
 냉장함 구동
 MOTOR0 = 5840_3650_40RPM
 MOTOR1 = JGB37_3525_45RPM
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
int switch0;  //Telescopic_drive_base에 부착되어, 함이 닫혔을 때, 눌린상태(0) 값 유지
int switch1;  //Cooling-cell-push-bar가 원점이면, 눌린상태(0) 값 유지

int MOTOR0testms = 20;
int MOTOR0testpersist = 1600;
int MOTOR0testdelay = 4000;
int MOTOR1testms = 20;          //수정
int MOTOR1testpersist = 2300;   //수정
int MOTOR1testdelay = 4000;     //수정

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

  pinMode(6, INPUT_PULLUP); //switch0;눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  pinMode(12, INPUT_PULLUP); //switch1;
  
  Serial.println("type B >>> MOTOR0 suddenCW");
  Serial.println("type N >>> MOTOR0 suddenCCW");
  Serial.println("type M >>> MOTOR0 suddenSTOP");
  Serial.println("type J >>> MOTOR1 suddenCW");
  Serial.println("type K >>> MOTOR1 suddenCCW");
  Serial.println("type L >>> MOTOR1 suddenSTOP");
  Serial.println("type Z >>> MOTOR0 CW");
  Serial.println("type X >>> MOTOR0 CCW");
  Serial.println("type C >>> MOTOR0 STOP");
  Serial.println("type A >>> MOTOR1 CW");
  Serial.println("type S >>> MOTOR1 CCW");
  Serial.println("type D >>> MOTOR1 STOP");
  Serial.println("type T >>> MOTOR0 TEST");
  Serial.println("type Y >>> MOTOR1 TEST");
  Serial.println("type R >>> switchread");
  Serial.println("type G >>> SUBTEST");
  Serial.println("type Q >>> MOTOR0OPEN");
  Serial.println("type W >>> MOTOR0CLOSE");
  Serial.println("type O >>> MOTOR1OPEN");
  Serial.println("type P >>> MOTOR1CLOSE");
}

void MOTOR0OPEN()
{
  MOTOR0CW();
  delay(MOTOR0testpersist);
  MOTOR0STOP();
  Serial.println("0,Q,0");
}

void MOTOR0CLOSE()
{
  MOTOR0CCW();  
  while(switch0 == 1){
    Serial.print("");    
  }
  MOTOR0STOP();
  Serial.println("0,W,0");
}

void MOTOR1OPEN()
{
  MOTOR1CW();
  delay(MOTOR1testpersist);
  MOTOR1STOP();
  Serial.println("1,O,0");
}

void MOTOR1CLOSE()
{
  MOTOR1CCW();
  while(switch1 == 1){
    Serial.print("");    
  }
  MOTOR1STOP();
  Serial.println("1,P,0");
}

void SUBTEST()
{
  int c = 5;
  while(c > 0){
    MOTOR1suddenCW();
    delay(500);
    MOTOR1suddenSTOP();
    delay(1000);
    MOTOR1suddenCCW();
    while(switch1 == 1){
      Serial.print("");    
    }
    MOTOR1suddenSTOP();
    delay(1000);
    c -= 1;
  }
}

void MOTOR0TEST()
{
  delay(5000);
  int cnt = 0;
  
  while(cnt < 1){
    cnt += 1;
    
    MOTOR0CW();
    delay(MOTOR0testpersist);
    MOTOR0STOP();
    delay(MOTOR0testdelay);
    MOTOR0CCW();
    
    delay(MOTOR0testpersist+150);
    /*
    while(switch1 == 1){
      Serial.print("");
    }
    */
    
    MOTOR0STOP();
    Serial.println(cnt);
    delay(MOTOR0testdelay);
  }
}

void MOTOR1TEST()
{
  int cnt = 0;
  while(cnt < 1){
    cnt += 1;
    
    MOTOR1CW();
    delay(MOTOR1testpersist);
    MOTOR1STOP();
    delay(MOTOR1testdelay);
    MOTOR1CCW();
    
    delay(MOTOR1testpersist);
    /*
    while(switch2 == 1){
      Serial.print("");
    }*/
    
    MOTOR1STOP();
    Serial.println(cnt);
    delay(MOTOR1testdelay);
  }
}

void switchcheck() {
  switch0 = digitalRead(6);
  switch1 = digitalRead(12);
}

void switchread() {
  Serial.print("switch0 = ");
  Serial.println(switch0);
  Serial.print("switch1 = ");
  Serial.println(switch1);
}

void MOTOR0suddenCW()
{  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, LOW);
  analogWrite(ENA, 255);
}

void MOTOR0suddenCCW()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, HIGH);
  analogWrite(ENA, 255);
}

void MOTOR0suddenSTOP()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void MOTOR1suddenCW()
{  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, LOW);
  analogWrite(ENB, 255);
}

void MOTOR1suddenCCW()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, HIGH);
  analogWrite(ENB, 255);
}

void MOTOR1suddenSTOP()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void MOTOR0CW()
{  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, LOW);
  for(i = 0 ; i <= a ; i=i+5){
    analogWrite(ENA, i);
    delay(MOTOR0testms);
  }
}

void MOTOR0CCW()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, HIGH);
  for(i = 0 ; i <= a ; i=i+5){
    analogWrite(ENA, i);
    delay(MOTOR0testms);
  }
}

void MOTOR0STOP()
{
  for(i = a ; i >= 0 ; i=i-5){
    analogWrite(ENA, i);
    delay(MOTOR0testms);
  }
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void MOTOR1CW()
{  
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, LOW);
  for(i = 100 ; i <= a ; i=i+5){
    analogWrite(ENB, i);
    delay(MOTOR1testms);
  }
}

void MOTOR1CCW()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, HIGH);
  for(i = 100 ; i <= a ; i=i+5){
    analogWrite(ENB, i);
    delay(MOTOR1testms);
  }
}

void MOTOR1STOP()
{
  for(i = a ; i >= 0 ; i=i-5){
    analogWrite(ENB, i);
    delay(MOTOR1testms);
  }
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'B') {
      MOTOR0suddenCW();
    }
    else if (input == 'N') {
      MOTOR0suddenCCW();
    }
    else if (input == 'M') {
      MOTOR0suddenSTOP();
    }
    else if (input == 'J') {
      MOTOR1suddenCW();
    }
    else if (input == 'K') {
      MOTOR1suddenCCW();
    }
    else if (input == 'L') {
      MOTOR1suddenSTOP();
    }
    else if (input == 'Z') {
      MOTOR0CW();
    }
    else if (input == 'X') {
      MOTOR0CCW();
    }
    else if (input == 'C') {
      MOTOR0STOP();
    }
    else if (input == 'A') {
      MOTOR1CW();
    }
    else if (input == 'S') {
      MOTOR1CCW();
    }
    else if (input == 'D') {
      MOTOR1STOP();
    }
    else if (input == 'T') {
      MOTOR0TEST();
    }
    else if (input == 'Y') {
      MOTOR1TEST();
    }
    else if (input == 'R') {
      switchread();
    }
    else if (input == 'G') {
      SUBTEST();
    }
    else if (input == 'Q') {
      MOTOR0OPEN();
    }
    else if (input == 'W') {
      MOTOR0CLOSE();
    }
    else if (input == 'O') {
      MOTOR1OPEN();
    }
    else if (input == 'P') {
      MOTOR1CLOSE();
    }
  }
}
