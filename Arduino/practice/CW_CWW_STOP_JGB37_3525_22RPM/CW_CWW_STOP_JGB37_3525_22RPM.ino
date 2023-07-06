#define ENB 9    //ENB핀으로 PWM 관리 됨.
#define IN3 26
#define IN4 27
#define PWM 10    //PWM 관리 안됨
#define relay 31
#define FG 11

int pos = 0;
int i = 0;
int a = 255;  //초기속도 가장 빠른 값
int currentspeed = 0; //현재속도

/*
  relay 시계방향 : LOW, 반시계방향 : HIGH

  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, LOW);   >>> ON

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);   >>> OFF

  //JGB37-3525 Speed SETING 세팅 불가.
  //JGB37-3525 a : 255에서만 속도측정 신뢰 가능. 약18rpm
*/

void setup() {
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(FG, INPUT);

  Serial.begin(9600);
  Serial.print("type Q >>> CW\n");
  Serial.print("type W >>> CCW\n");
  Serial.print("type E >>> STOP\n");
  Serial.print("type A >>> Speed Check\n");
  Serial.print("type Z >>> Speed UP\n");
  Serial.print("type X >>> Speed DOWN\n");
  Serial.print("type C >>> Speed RESET\n");
  Serial.print("type S >>> Speed SETING\n");
}

void CW()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay, LOW);
  analogWrite(ENB, 255);
  /*
  for(i = 105 ; i <= a ; i=i+10){
    analogWrite(ENB, i);
    delay(40);
  }
  */
}

void CWtostop()
{
  analogWrite(ENB, 55);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay, LOW);
}

void CCW()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay, HIGH);
  analogWrite(ENB, 255);
  /*
  for(i = 105 ; i <= a ; i=i+10){
    analogWrite(ENB, i);
    delay(40);
  }
  */
}

void CCWtostop()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay, HIGH);
  analogWrite(ENB, 55);
}

void STOP()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void suddenSTOP()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void Speedcheck(){
  int c = 0;
  
  for(int k = 0; k<100 ;k++){
    for(int j = 0;j<8;j++){
     i += pulseIn(FG, HIGH, 500000); //cycle = 6*i,1s = 1000000us
    }                                //8
    i = i >> 3;                      //i*1000
    c += 14500/i;                    //c : 24V 40RPM 기준으로 측정되는 100개의 값의 합
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
    c += 14500/i;                    //c : 24V 40RPM 기준으로 측정되는 100개의 값의 합
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
  }
}
