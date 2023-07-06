/* 
  택배함 구동
  MOTOR2 = JGA25_2430_25RPM
  클로젯함 구동
  MOTOR3 = JGA25_2430_25RPM
*/

#include <MsTimer2.h>  //실시간으로 스위치 상태 체크하기위한 라이브러리
#include <Wire.h>

#define IN1 24
#define IN2 25
#define IN3 26
#define IN4 27
#define ENA 2
#define ENB 3
#define relay1 4
#define relay2 5

//아래는 보드에 모터, 스위치 PIN 연결
#define PWM1 6
#define FG1 7
#define PWM2 8
#define FG2 9
#define SW2 10
#define SW3 11

/*
  PWM(255 ~ 0) : 속도 제어
  속도 가장 빠른 값 = 0, 속도 느리게 조절한 값 : 190
  
  relay 시계방향 : LOW, 반시계방향 : HIGH

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);   >>> OFF

  analogWrite(PWM, 0);      >>> FAST
  analogWrite(PWM, 255);    >>> STOP
  (주의)

int rr = 2450; // 정격속도[pwm 255]로 한바퀴도는데 걸리는 시간[ms]
int ar = 62;  //정격속도[pwm 255]까지 가속 또는 감속시 한바퀴 도는데 걸리는 시간[ms]
*/

int switch2;  //택배함에 부착되어, 함이 닫혔을 때, 눌린상태(0) 값 유지
int switch3;  //옷장함가 원점이면, 눌린상태(0) 값 유지
char msg[6];

int MOTOR2testms = 20;
int MOTOR2testpersist = 2700;
int MOTOR3testms = 20;
int MOTOR3testpersist = 2700;

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

  pinMode(SW2, INPUT_PULLUP); //switch2;눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  pinMode(SW3, INPUT_PULLUP); //switch3;

  /*
  - TEST -
  Serial.println("type R >>> switchread");
  Serial.println("type Q >>> MOTOR2 CW");
  Serial.println("type W >>> MOTOR2 CCW");
  Serial.println("type E >>> MOTOR2 STOP");
  Serial.println("type B >>> MOTOR3 CW");
  Serial.println("type N >>> MOTOR3 CCW");
  Serial.println("type M >>> MOTOR3 STOP");

  - APPLY -
  Serial.println("type H >>> MOTOR2 OPEN");
  Serial.println("type J >>> MOTOR2 CLOSE");
  Serial.println("type K >>> MOTOR3 OPEN");
  Serial.println("type L >>> MOTOR3 CLOSE");
  Serial.println("type Z >>> MOTOR2 suddenSTOP");
  Serial.println("type X >>> MOTOR3 suddenSTOP");
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

void MOTOR2OPEN()
{
  msg[0] = '2';
  msg[1] = ',';
  msg[2] = 'H';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  MOTOR2CCW();
  wait(MOTOR2testpersist);
  MOTOR2STOP();
  
  msg[0] = '2';
  msg[1] = ',';
  msg[2] = 'H';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR2CLOSE()
{
  msg[0] = '2';
  msg[1] = ',';
  msg[2] = 'J';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  MOTOR2CW();
  while(switch2 == 1){
    char req = Serial.read();
    brocker(req);
  }
  MOTOR2STOP();
  
  msg[0] = '2';
  msg[1] = ',';
  msg[2] = 'J';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR3OPEN()
{
  msg[0] = '3';
  msg[1] = ',';
  msg[2] = 'K';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  MOTOR3CCW();
  wait(MOTOR3testpersist);
  MOTOR3STOP();
  
  msg[0] = '3';
  msg[1] = ',';
  msg[2] = 'K';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR3CLOSE()
{
  msg[0] = '3';
  msg[1] = ',';
  msg[2] = 'L';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  MOTOR3CW();
  while(switch3 == 1){
    char req = Serial.read();
    brocker(req);  
  }
  MOTOR3STOP();
  
  msg[0] = '3';
  msg[1] = ',';
  msg[2] = 'L';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR2suddenSTOP()
{
  msg[0] = '2';
  msg[1] = ',';
  msg[2] = 'Z';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  analogWrite(ENA, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  msg[0] = '2';
  msg[1] = ',';
  msg[2] = 'Z';
  msg[3] = ',';
  msg[4] = '0';
}

void MOTOR3suddenSTOP()
{
  msg[0] = '3';
  msg[1] = ',';
  msg[2] = 'X';
  msg[3] = ',';
  msg[4] = '1';
  Serial.println(msg);
  
  analogWrite(ENA, 255);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  msg[0] = '3';
  msg[1] = ',';
  msg[2] = 'X';
  msg[3] = ',';
  msg[4] = '0';
}

void switchcheck() {
  switch2 = digitalRead(SW2);
  switch3 = digitalRead(SW3);
}

void switchread() {
  /*
  Serial.print(switch2);
  Serial.print(",");
  Serial.println(switch3);
  */
}

void MOTOR2CW()
{
  analogWrite(ENA, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, LOW);
  
  for(int i = 190 ; i >= 0 ; i=i-5){
    analogWrite(PWM1, i);
    delay(MOTOR2testms);
  }
}

void MOTOR2CCW()
{
  analogWrite(ENA, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, HIGH);
  
  for(int i = 190 ; i >= 0 ; i=i-5){
    analogWrite(PWM1, i);
    delay(MOTOR2testms);
  }
}

void MOTOR2STOP()
{
  for(int i = 0 ; i <= 255 ; i=i+5){
    analogWrite(PWM1, i);
    delay(MOTOR2testms);
  }
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void MOTOR3CW()
{
  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, LOW);
  
  for(int i = 190 ; i >= 0 ; i=i-5){
    analogWrite(PWM2, i);
    delay(MOTOR3testms);
  }
}

void MOTOR3CCW()
{
  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, HIGH);
  
  for(int i = 190 ; i >= 0 ; i=i-5){
    analogWrite(PWM2, i);
    delay(MOTOR3testms);
  }
}

void MOTOR3STOP()
{
  for(int i = 0 ; i <= 255 ; i=i+5){
    analogWrite(PWM2, i);
    delay(MOTOR3testms);
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
  else if (req == 'Q')
  {
    MOTOR2CW();
  }
  else if (req == 'W')
  {
    MOTOR2CCW();
  }
  else if (req == 'E')
  {
    MOTOR2STOP();
  }
  else if (req == 'B')
  {
    MOTOR3CW();
  }
  else if (req == 'N')
  {
    MOTOR3CCW();
  }
  else if (req == 'M')
  {
    MOTOR3STOP();
  }
  else if (req == 'H')
  {
    MOTOR2OPEN();
    Serial.println(msg);
  }
  else if (req == 'J')
  {
    MOTOR2CLOSE();
    Serial.println(msg);
  }
  else if (req == 'K')
  {
    MOTOR3OPEN();
    Serial.println(msg);
  }
  else if (req == 'L')
  {
    MOTOR3CLOSE();
    Serial.println(msg);
  }
  else if (req == 'Z')
  {
    MOTOR2suddenSTOP();
    Serial.println(msg);
  }
  else if (req == 'X')
  {
    MOTOR3suddenSTOP();
    Serial.println(msg);
  }
}

void loop() {
  char req = Serial.read();
  brocker(req);
}
