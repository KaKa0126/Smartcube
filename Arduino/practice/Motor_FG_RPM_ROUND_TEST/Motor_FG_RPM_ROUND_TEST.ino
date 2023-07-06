/*
 * l = [rpm]*T[ms]
 * 5840-3650 24V 80RPM >>> 변환 상수 l = 103.32
 * 5840-3650 24V 40RPM >>> 변환 상수 l = 53.3, s = 0.0000041;
 */

#include <MsTimer2.h>  //실시간으로 스위치 상태 체크하기위한 라이브러리
#include <Wire.h>

#define IN1 24
#define IN2 25
#define ENA 2
#define relay1 4
#define PWM1 6
#define FG1 7

unsigned long value[50];
float k = 10; //짝수(k값이 변하면 '코드 읽는 시간'이 길어질 수 있음.)
float sum = 0;  //측정값1+측정값2
float sum1 = 0; //측정값1
float sum2 = 0; //측정값2
float l = 53.3;
float b = 0;    //측정 평균1
float d = 0;    //측정 평균2
float s = 0.055;  //측정 주기(motorcheck timer 주기(20[ms]) + 코드 읽는 시간(35[ms]))
//(a,b),(c,d), s = c-a
float r;  //r = [round]
int cnt = 0;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(relay1, OUTPUT);
  pinMode(PWM1, OUTPUT);
  pinMode(FG1, INPUT);
  
  Serial.begin(9600);

  MsTimer2::set(20, motorcheck); //ms(20 + 35(코드읽는 시간))마다 스위치 상태 측정하겠다.
  MsTimer2::start();
  
  Serial.println("ms,rpm,round");
}

void motorcheck(){
  sum = 0;
  sum1 = 0;
  sum2 = 0;
  b = 0;
  d = 0;

  for(int i = 0 ; i < k ; i++){
    value[i] = pulseIn(FG1,HIGH,20000);
    sum += value[i];   
    
    if(i < (k/2)){
      sum1 += value[i];
    }
    else{
      sum2 += value[i];
    }
  }
  
  b += 1/(sum1/(k/2)*2/1000)*l;
  d += 1/(sum2/(k/2)*2/1000)*l;
  
  Serial.print("b = ");
  Serial.print(b);
  Serial.print(", d = ");
  Serial.println(d);

  Serial.print(sum/k*2/1000);  //주기이므로 *2, 단위는 ms이므로 *1000, [ms]
  Serial.print(",");

  if(1/(sum/k*2/1000)*l < 45){
    Serial.print(1/(sum/k*2/1000)*l);  //주기이므로 *2, 단위는 ms이므로 *1000, [rpm]
    Serial.print(",");

    r += (b+d)*s/2/60;
    if (cnt == 0){
      r = 0;
      cnt = 1;
    }
    Serial.println(r);
  }
  else{
    Serial.print(0);
    Serial.print(",");
    Serial.println(0);
    cnt = 0;
  }
}

void MOTORCW(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, HIGH);
  analogWrite(ENA, 255);
  analogWrite(PWM1, 255);
}

void MOTORCCW(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay1, LOW);
  analogWrite(ENA, 255);
  analogWrite(PWM1, 255);
}

void STOP(){
  analogWrite(ENA, 0);
  analogWrite(PWM1, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void ROUND_TEST(){
  MOTORCW();
  while(r < 20){
    Serial.print("");
  }
  STOP();
  r = 0;
}


void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'Q') {
      MOTORCW();
    }
    else if (input == 'W') {
      MOTORCCW();
    }
    else if (input == 'E') {
      STOP();
    }
    else if (input == 'T') {
      ROUND_TEST();
    }
  }
}
