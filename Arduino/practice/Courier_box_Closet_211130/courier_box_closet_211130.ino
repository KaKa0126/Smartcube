/* 
  택배함 구동
  MOTOR2 = JGA25_2430_25RPM
  클로젯함 구동
  MOTOR3 = JGA25_2430_25RPM
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
int a = 0;  //초기속도 가장 빠른 값 = 0, 190은 속도 느리게 조절한 값
int MOTOR2currentspeed = 0; //현재속도
int rr = 2450; // 정격속도[pwm 255]로 한바퀴도는데 걸리는 시간[ms]
int ar = 62;  //정격속도[pwm 255]까지 가속 또는 감속시 한바퀴 도는데 걸리는 시간[ms]
int MOTOR2testpersist = 2700;
int MOTOR3testpersist = 2700;
int switch2;  //택배함에 부착되어, 함이 닫혔을 때, 눌린상태(0) 값 유지
int switch3;  //옷장함가 원점이면, 눌린상태(0) 값 유지

/*
  relay 시계방향 : LOW, 반시계방향 : HIGH

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);   >>> OFF

  analogWrite(PWM, 0);      >>> FAST
  analogWrite(PWM, 255);    >>> STOP
  (주의)
*/

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

  pinMode(6, INPUT_PULLUP); //switch2;눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  pinMode(12, INPUT_PULLUP); //switch3;
  
  Serial.print("type Q >>> MOTOR2 CW\n");
  Serial.print("type W >>> MOTOR2 CCW\n");
  Serial.print("type E >>> MOTOR2 STOP\n");
  Serial.print("type B >>> MOTOR3 CW\n");
  Serial.print("type N >>> MOTOR3 CCW\n");
  Serial.print("type M >>> MOTOR3 STOP\n");
  Serial.print("type A >>> MOTOR2 Speed Check\n");
  Serial.print("type Z >>> MOTOR2 Speed UP\n");
  Serial.print("type X >>> MOTOR2 Speed DOWN\n");
  Serial.print("type C >>> MOTOR2 Speed RESET\n");
  Serial.print("type S >>> MOTOR2 Speed SETING\n");
  Serial.print("type H >>> MOTOR2 OPEN\n");
  Serial.print("type J >>> MOTOR2 CLOSE\n");
  Serial.print("type K >>> MOTOR3 OPEN\n");
  Serial.print("type L >>> MOTOR3 CLOSE\n");
  Serial.print("type T >>> TEST\n");
}

void switchcheck() {
  switch2 = digitalRead(6);
  switch3 = digitalRead(12);
}

void TEST()
{
  
}

void MOTOR2OPEN()
{
  MOTOR2CCW(); //1바퀴 가속(기본세팅)
  delay(MOTOR2testpersist);
  MOTOR2STOP(); //1바퀴 감속(기본세팅)
  Serial.println("2,H,0");
}

void MOTOR2CLOSE()
{
  MOTOR2CW(); //1바퀴 가속(기본세팅)
  while(switch2 == 1){
    Serial.print("");    
  }
  MOTOR2STOP(); //1바퀴 감속(기본세팅)
  Serial.println("2,J,0");
}

void MOTOR3OPEN()
{
  MOTOR3CCW(); //1바퀴 가속(기본세팅)
  delay(MOTOR3testpersist);
  MOTOR3STOP(); //1바퀴 감속(기본세팅)
  Serial.println("3,K,0");
}

void MOTOR3CLOSE()
{
  MOTOR3CW(); //1바퀴 가속(기본세팅)
  while(switch3 == 1){
    Serial.print("");    
  }
  MOTOR3STOP(); //1바퀴 감속(기본세팅)
  Serial.println("3,L,0");
}

void MOTOR2CW()
{
  analogWrite(ENA, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, LOW);
  analogWrite(PWM1, a);
}

void MOTOR3CW()
{
  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, LOW);
  analogWrite(PWM2, a);
}

void MOTOR2CWtostop()
{
  analogWrite(ENA, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, LOW);
  analogWrite(PWM1, 200);
}

void MOTOR2CCW()
{
  analogWrite(ENA, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, HIGH);
  analogWrite(PWM1, a);
}

void MOTOR3CCW()
{
  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay2, HIGH);
  analogWrite(PWM2, a);
}

void MOTOR2STOP()
{
  analogWrite(PWM1, 255);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void MOTOR3STOP()
{
  analogWrite(PWM2, 255);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void MOTOR2CCWtostop()
{
  analogWrite(ENA, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, HIGH);
  analogWrite(PWM1, 200);
}

void MOTOR2suddenSTOP()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void MOTOR3suddenSTOP()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void MOTOR2Speedcheck(){
  int c = 0;
  
  for(int k = 0; k<100 ;k++){
    for(int j = 0;j<8;j++){
     i += pulseIn(FG1, HIGH, 500000); //cycle = 6*i,1s = 1000000us
    }                                //8
    i = i >> 3;                      //i*1000
    c += 17000/i;                    //c : 24V 40RPM 기준으로 측정되는 100개의 값의 합
  }
  MOTOR2currentspeed = c/100;
  Serial.print(c/100); //speed   r/min
  Serial.println("  r/min");
} //속도 계산 함수

void MOTOR2SpeedUP(){
  MOTOR2STOP();
  if(a <= 255){
    a -= 25;
  }
}

void MOTOR2SpeedDOWN(){
  MOTOR2STOP();
  if(a >= 0){
    a += 25;
  }
}

void MOTOR2SpeedRESET(){
  MOTOR2STOP();
  if(a >= 0){
    a = 0;
  }
}

void MOTOR2SpeedSETING(){
  int userin = 24;       //속도 설정시 사용자가 입력하는 값. 기본값 : 24
  MOTOR2Speedcheck();
  Serial.print("The current spped : ");
  Serial.print(MOTOR2currentspeed);
  Serial.println(" r/min");  
  Serial.print("Please enter the speed(r/min) you want within 10 seconds.(caution! 19~24)\n");
  delay(10000);
  while (Serial.available()){
    userin = Serial.parseInt();
    Serial.println(userin);
  }
  while(userin != MOTOR2currentspeed){    
    if(userin > MOTOR2currentspeed){
      if(a > 20){
        a -= 5;
        Serial.println("speed up");
      }
    }
    else{
      if(a < 200){
        a += 5;
        Serial.println("speed down");
      }
    }
    MOTOR2STOP();
    MOTOR2CW();
    MOTOR2Speedcheck();    
  }
  Serial.println("DONE");
}

void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'Q') {
      MOTOR2CW();
    }
    else if (input == 'W') {
      MOTOR2CCW();
    }
    else if (input == 'E') {
      MOTOR2STOP();
    }
    else if (input == 'B') {
      MOTOR3CW();
    }
    else if (input == 'N') {
      MOTOR3CCW();
    }
    else if (input == 'M') {
      MOTOR3STOP();
    }
    else if (input == 'A') {
      MOTOR2Speedcheck();
    }
    else if (input == 'Z') {
      MOTOR2SpeedUP();
    }
    else if (input == 'X') {
      MOTOR2SpeedDOWN();
    }
    else if (input == 'C') {
      MOTOR2SpeedRESET();
    }
    else if (input == 'S') {
      MOTOR2SpeedSETING();
    }
    else if (input == 'T') {
      TEST();
    }
    else if (input == 'H') {
      MOTOR2OPEN();
    }
    else if (input == 'J') {
      MOTOR2CLOSE();
    }
    else if (input == 'K') {
      MOTOR3OPEN();
    }
    else if (input == 'L') {
      MOTOR3CLOSE();
    }
  }
}
