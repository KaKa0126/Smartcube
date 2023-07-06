//PWM 값에 따라 모터의 RPM을 시리얼 모니터로 측정하는 코드
#define ENB 10
#define IN3 22
#define IN4 23
#define PWM 12
#define relay 26
#define FG 2

int pos = 0;
int i = 0;

/*
  relay 시계방향 : LOW, 반시계방향 : HIGH

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);   >>> ON

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);   >>> OFF
*/

void setup() {
  Serial.begin(9600);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(relay, OUTPUT);
  pinMode(FG, INPUT);     //모터에서 주는 신호를 받아서 사용
}

void CW()
{
  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay, LOW);
  for(i = 5; i <= 255 ; i=i+10){
    analogWrite(PWM, i);
    delay(10);
  }
}

void CWtostop()
{
  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay, LOW);
  analogWrite(PWM, 50);
}

void CCW()
{
  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay, HIGH);
  for(i = 5; i <= 255 ; i=i+10){
    analogWrite(PWM, i);
    delay(10);
  }  
}

void CCWtostop()
{
  analogWrite(ENB, 255);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(relay, HIGH);
  analogWrite(PWM, 50);
}

void STOP()
{
  for(i = 250; i >= 0 ; i=i-10){
    analogWrite(PWM, i);
    delay(10);
  }
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
     Serial.println(i);
    }                                //8
    i = i >> 3;                      //i*1000
    c += 30000/i;                    //c : 24V 40RPM 기준으로 측정되는 100개의 값의 합
  }
  Serial.print(c/100); //speed   r/min
  Serial.println("  r/min");
} //속도 계산 함수

void prac(){
  Serial.println(FG);
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
      Speedcheck();
    }
    else if (input == 'A') {
      prac();
    }
  }
}
