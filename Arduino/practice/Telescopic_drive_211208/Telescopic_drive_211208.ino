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

//PWM : 0 ~ 255, 속도 가장 빠른 값 : 255

int switch0;  //Telescopic_drive_base에 부착되어, 함이 닫혔을 때, 눌린상태(0) 값 유지
int switch1;  //Cooling-cell-push-bar가 원점이면, 눌린상태(0) 값 유지
char msg[6];

int MOTOR0testms = 20;
int MOTOR0testpersist = 1600;
int MOTOR1testms = 20;
int MOTOR1testpersist = 2300;

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

  /*
  - TEST -
  Serial.println("type R >>> switchread");
  Serial.println("type Z >>> MOTOR0 CW");
  Serial.println("type X >>> MOTOR0 CCW");
  Serial.println("type C >>> MOTOR0 STOP");
  Serial.println("type A >>> MOTOR1 CW");
  Serial.println("type S >>> MOTOR1 CCW");
  Serial.println("type D >>> MOTOR1 STOP");

  - APPLY -
  Serial.println("type Q >>> MOTOR0OPEN");
  Serial.println("type W >>> MOTOR0CLOSE");
  Serial.println("type O >>> MOTOR1OPEN");
  Serial.println("type P >>> MOTOR1CLOSE");
  Serial.println("type M >>> MOTOR0 suddenSTOP");
  Serial.println("type L >>> MOTOR1 suddenSTOP");
  */
}

void wait(unsigned long duration_ms)
{
  unsigned long time_start = millis();
  unsigned long time_end = time_start;
  bool is_complete = false;
  while (!is_complete)
  {
    // 메시지 처리
    char req = Serial.read();
    brocker(req);

    unsigned long time_current = millis();

    if (time_current - time_start >= duration_ms)
    {
      time_end = millis();
      is_complete = true;
    }
  }
}

void MOTOR0OPEN()
{
  msg[0] = '0';
  msg[1] = ',';
  msg[2] = 'Q';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  MOTOR0CW();
  wait(MOTOR0testpersist);
  MOTOR0STOP();
  
  msg[0] = '0';
  msg[1] = ',';
  msg[2] = 'Q';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR0CLOSE()
{
  msg[0] = '0';
  msg[1] = ',';
  msg[2] = 'W';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  MOTOR0CCW();
  while (switch0 == 1) {
    char req = Serial.read();
    brocker(req);
  }
  MOTOR0STOP();
  
  msg[0] = '0';
  msg[1] = ',';
  msg[2] = 'W';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR1OPEN()
{
  msg[0] = '1';
  msg[1] = ',';
  msg[2] = 'O';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  MOTOR1CW();
  wait(MOTOR1testpersist);
  MOTOR1STOP();
  
  msg[0] = '1';
  msg[1] = ',';
  msg[2] = 'O';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR1CLOSE()
{
  msg[0] = '1';
  msg[1] = ',';
  msg[2] = 'P';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  MOTOR1CCW();
  while (switch1 == 1) {
    char req = Serial.read();
    brocker(req);
  }
  MOTOR1STOP();
  
  msg[0] = '1';
  msg[1] = ',';
  msg[2] = 'P';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR0suddenSTOP()
{
  msg[0] = '0';
  msg[1] = ',';
  msg[2] = 'M';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  analogWrite(ENA, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  msg[0] = '0';
  msg[1] = ',';
  msg[2] = 'M';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR1suddenSTOP()
{
  msg[0] = '1';
  msg[1] = ',';
  msg[2] = 'L';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  analogWrite(ENB, 0);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  msg[0] = '1';
  msg[1] = ',';
  msg[2] = 'L';
  msg[3] = ',';
  msg[4] = '0';
}

void switchcheck() {
  switch0 = digitalRead(6);
  switch1 = digitalRead(12);
}

void switchread() {
  /*
  Serial.print(switch0);
  Serial.print(",");
  Serial.println(switch1);
  */
}

void MOTOR0CW()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, LOW);
  
  for(int i = 0 ; i <= 255 ; i=i+5){
    analogWrite(ENA, i);
    delay(MOTOR0testms);
  }
}

void MOTOR0CCW()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, HIGH);
  
  for(int i = 0 ; i <= 255 ; i=i+5){
    analogWrite(ENA, i);
    delay(MOTOR0testms);
  }
}

void MOTOR0STOP()
{
  for(int i = 255 ; i >= 0 ; i=i-5){
    analogWrite(ENA, i);
    delay(MOTOR1testms);
  }
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void MOTOR1CW()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, LOW);
  
  for(int i = 100 ; i <= 255 ; i=i+5){
    analogWrite(ENB, i);
    delay(MOTOR1testms);
  }
}

void MOTOR1CCW()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, HIGH);
  
  for(int i = 100 ; i <= 255 ; i=i+5){
    analogWrite(ENB, i);
    delay(MOTOR1testms);
  }
}

void MOTOR1STOP()
{
  for(int i = 255 ; i >= 0 ; i=i-5){
    analogWrite(ENB, i);
    delay(MOTOR1testms);
  }
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void brocker(char req)
{
  if (req == 'R')
  {
    switchread();
  }
  else if (req == 'Z')
  {
    MOTOR0CW();
  }
  else if (req == 'X')
  {
    MOTOR0CCW();
  }
  else if (req == 'C')
  {
    MOTOR0STOP();
  }
  else if (req == 'A')
  {
    MOTOR1CW();
  }
  else if (req == 'S')
  {
    MOTOR1CCW();
  }
  else if (req == 'D')
  {
    MOTOR1STOP();
  }
  else if (req == 'Q')
  {
    MOTOR0OPEN();
    Serial.println(msg);
  }
  else if (req == 'W')
  {
    MOTOR0CLOSE();
    Serial.println(msg);
  }
  else if (req == 'O')
  {
    MOTOR1OPEN();
    Serial.println(msg);
  }
  else if (req == 'P')
  {
    MOTOR1CLOSE();
    Serial.println(msg);
  }
  else if (req == 'M')
  {
    MOTOR0suddenSTOP();
    Serial.println(msg);
  }
  else if (req == 'L')
  {
    MOTOR1suddenSTOP();
    Serial.println(msg);
  }
}

void loop() {
  char req = Serial.read();
  brocker(req);
}
