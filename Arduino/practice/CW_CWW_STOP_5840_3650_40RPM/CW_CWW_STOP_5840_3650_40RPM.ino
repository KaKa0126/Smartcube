#define ENA 3    //ENA핀으로 PWM 관리 됨
#define IN1 24
#define IN2 25
#define PWM 4
#define relay 30
#define FG 5

int pos = 0;
int i = 0;
int a = 255;  //초기속도 가장 빠른 값
int currentspeed = 0; //현재속도
int rr = 1460; // 정격속도[pwm 255]로 한바퀴도는데 걸리는 시간[ms]
int ar = 34;  //정격속도[pwm 255]까지 가속 또는 감속시 한바퀴 도는데 걸리는 시간[ms]
int sr = 31;

int testms = 20;
int testpersist = 1600;
int testdelay = 4000;

/*
  relay 시계방향 : LOW, 반시계방향 : HIGH

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);   >>> ON

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);   >>> OFF

  //5840_3650 Speed SETING 작동 주의
  //5840_3650 a : 255~130 값에서 속도측정 신뢰 가능. 약40rpm
*/

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);  
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(FG, INPUT);

  Serial.begin(9600);
  Serial.print("type Q >>> CW\n");
  Serial.print("type W >>> CCW\n");
  Serial.print("type E >>> STOP\n");
  Serial.print("type B >>> suddenCW\n");
  Serial.print("type N >>> suddenCCW\n");
  Serial.print("type A >>> Speed Check\n");
  Serial.print("type Z >>> Speed UP\n");
  Serial.print("type X >>> Speed DOWN\n");
  Serial.print("type C >>> Speed RESET\n");
  Serial.print("type S >>> Speed SETING\n");
  Serial.print("type T >>> TEST\n");
  Serial.print("type Y >>> TEST2\n");
}

void TEST()
{
  int cnt = 0;
  
  while(cnt < 1){
    cnt += 1;
    
    CW();
    delay(testpersist);
    STOP();
    delay(testdelay);
    CCW();
    delay(testpersist+150);
    STOP();
    Serial.println(cnt);
    delay(testdelay);
  }
}

void TEST2()
{
  CW();
  delay(testpersist);
  STOP();
}

void CW()
{  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay, LOW);
  for(i = 0 ; i <= a ; i=i+5){
    analogWrite(ENA, i);
    delay(testms);
  }
}

void suddenCW()
{  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay, LOW);
  analogWrite(ENA, 255);
}

void CWtostop()
{
  analogWrite(ENA, 55);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay, LOW);
}

void CCW()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay, HIGH);
  for(i = 0 ; i <= a ; i=i+5){
    analogWrite(ENA, i);
    delay(testms);
  }
}

void suddenCCW()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay, HIGH);
  analogWrite(ENA, 255);
}

void CCWtostop()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay, HIGH);
  analogWrite(ENA, 55);
}

void STOP()
{
  for(i = a ; i >= 0 ; i=i-5){
    analogWrite(ENA, i);
    delay(testms);
  }
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void suddenSTOP()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void Speedcheck(){
  int c = 0;
  
  for(int k = 0; k<100 ;k++){
    for(int j = 0;j<8;j++){
     i += pulseIn(FG, HIGH, 500000); //cycle = 6*i,1s = 1000000us
    }                                //8
    i = i >> 3;                      //i*1000
    c += 31500/i;                    //c : 24V 40RPM 기준으로 측정되는 100개의 값의 합
  }
  currentspeed = c/100;
  Serial.print(c/100); //speed   r/min
  Serial.println("  r/min");
} //속도 계산 함수

void justSpeedcheck(){
  int c = 0;
  
  for(int k = 0; k<100 ;k++){
    for(int j = 0;j<8;j++){
     i += pulseIn(FG, HIGH, 500000); //cycle = 6*i,1s = 1000000us
    }                                //8
    i = i >> 3;                      //i*1000
    c += 31500/i;                    //c : 24V 40RPM 기준으로 측정되는 100개의 값의 합
  }
  currentspeed = c/100;
}

void SpeedUP(){
  STOP();
  if(a <= 230){
    a += 25;
  }
  else{
    a = 255;
  }
}

void SpeedDOWN(){
  STOP();
  if(a >= 25){
    a -= 25;
  }
  else{
    a = 0;
  }
}

void SpeedRESET(){
  STOP();
  a = 255;
}

void SpeedSETING(){
  int userin = 24;       //속도 설정시 사용자가 입력하는 값. 기본값 : 24
  justSpeedcheck();
  Serial.print("The current spped : ");
  Serial.print(currentspeed);
  Serial.println(" r/min");  
  Serial.print("Please enter the speed(r/min) you want within 10 seconds.(caution! 19~24)\n");
  delay(10000);
  while (Serial.available()){
    userin = Serial.parseInt();
    Serial.println(userin);
  }
  while(userin != currentspeed){    
    if(userin > currentspeed){
      if(a < 235){
        a += 5;
        Serial.println("speed up");
      }
    }
    else{
      if(a > 50){
        a -= 5;
        Serial.println("speed down");
      }
    }
    STOP();
    CW();
    justSpeedcheck();    
  }
  Serial.println("DONE");
}

void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'Q') {
      CW();
    }
    else if (input == 'W') {
      CCW();
    }
    else if (input == 'E') {
      STOP();
    }
    else if (input == 'B') {
      suddenCW();
    }
    else if (input == 'N') {
      suddenCCW();
    }
    else if (input == 'M') {
      suddenSTOP();
    }
    else if (input == 'A') {
      Speedcheck();
    }
    else if (input == 'Z') {
      SpeedUP();
    }
    else if (input == 'X') {
      SpeedDOWN();
    }
    else if (input == 'C') {
      SpeedRESET();
    }
    else if (input == 'S') {
      SpeedSETING();
    }
    else if (input == 'T') {
      TEST();
    }
    else if (input == 'Y') {
      TEST2();
    }
  }
}
