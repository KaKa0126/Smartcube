#define ENB 10
#define IN3 22
#define IN4 23
#define ENCA 21
#define ENCB 20

int pos = 0;
int i;
int a = 58;
//4초 역방향 : 140, 6초 역방향 : 73, 8초 역방향 : 58, 13초 역방향 : 45, 14.5초 역방향 : 43
//4초 정방향 : 138, 6초 정방향 : 72, 8초 정방향 : 58, 14.7초 정방향 : 43
int rr = 3598; // 정격속도[pwm 255]로 한바퀴도는데 걸리는 시간[ms]
int ar = 107;  //정격속도[pwm 255]까지 가속시 한바퀴 도는데 걸리는 시간[ms]
int sr = 91;  //정격속도[pwm 255]까지 감속시 한바퀴 도는데 걸리는 시간[ms]
int rrr = 1/3;  //rr의 한바퀴 기준
int arr = 1/3;  //ar의 한바퀴 기준
int srr = 1/3;  //sr의 한바퀴 기준

/*
  digitalWrite(IN3, HIGH);
  digitalWrite(IN2, LOW);   >>> ON

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);   >>> OFF
*/

void setup() {
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENCA, INPUT);
  pinMode(ENCB, INPUT);
  attachInterrupt(digitalPinToInterrupt(ENCA),readEncoder,RISING);

  Serial.begin(9600);
  Serial.print("type Q >>> CW\n");
  Serial.print("type W >>> CCW\n");
  Serial.print("type E >>> STOP\n");
  Serial.print("type R >>> Original position\n");
  Serial.print("type Z >>> Speed up\n");
  Serial.print("type X >>> Speed down\n");
  Serial.print("type C >>> Speed reset\n");
  Serial.print("type T >>> TEST\n");
}

void TEST()
{
  CW();
  delay(14700);
  suddenSTOP();
}

void CW()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, a);
  /*
  for(i = 0; i <= a ; i=i+5){
    analogWrite(ENB, i);
    delay(ar*arr);
  }
  */
}

void CWtostop()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENB, 50);
}

void CCW()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, a);
  /*
  for(i = 0; i <= a ; i=i+5){
    analogWrite(ENB, i);
    delay(ar*arr);
  }
  */
}

void CCWtostop()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENB, 50);
}

void STOP()
{
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  for(i = a; i >= 0 ; i=i-5){
    analogWrite(ENB, i);
    delay(sr*srr);
  }
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void suddenSTOP()
{
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void readEncoder(){
  int b = digitalRead(ENCB);
  if(b > 0){
    pos++;
  }
  else{
    pos--;
  }
}

void poschange(){
  while(pos > 3600 || pos < -3600){
    if(pos > 3600){
      pos -= 3600;
    }
    else{
      pos += 3600;
    }
  }
  Serial.print("각도 ");
  Serial.print(pos/10);   //필수코드
  Serial.println("°에서 원위치");
}

void Originalposition(){
  poschange();
  while(pos != 0){
    if(pos > 0){
      CCWtostop();
    }
    else{
      CWtostop();
    }
  }
  suddenSTOP();
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
    else if (input == 'R') {
      Originalposition();
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
    else if (input == 'T') {
      TEST();
    } 
  }
}
