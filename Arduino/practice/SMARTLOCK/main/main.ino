#include <MsTimer2.h>  //실시간으로 스위치 상태 체크하기위한 라이브러리
#include <Wire.h>

#define ENA 10
#define ENB 11
#define IN1 30
#define IN2 31
#define IN3 32
#define IN4 33


int sw1,sw2,sw3,asd = 0;
int start_num=0,num=0,err_num=0, cnt=0, close_bu = 0;
int motor_flag=3,open_flag=0,state_flag=3,err_flag=0;
int time_pre=0, time_cur=0, time_res=0, open_pre = 0, open_cur=0;
char mag_value, mag;


void setup() {
  motor_flag = 3;
  
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(115200);

  MsTimer2::set(10, swichcheck); //20ms마다 스위치 상태 측정하겠다.
  MsTimer2::start();

  pinMode(2, INPUT_PULLUP); //눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP); 
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);

  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}
void swichcheck() {
  sw1 = digitalRead(2);
  sw2 = digitalRead(3);
  sw3 = digitalRead(4);
}
void RESET(){
  num = 0;
  err_num = 0;
  open_flag = 0;
  time_pre = 0;
  time_cur = 0;
  time_res = 0;
  state_flag = 4;
  err_flag = 0;
  digitalWrite(5, LOW);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);
}
void LED_TWINKLE(){
  for(int i=0; i<5; i++){  
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    delay(300);
    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    delay(300);
  }
}
void CW(){
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}
void CCW(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, 255);
  analogWrite(ENB, 255);
}
void STOP(){
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
void PAUSE_C(){
  if(motor_flag==0){
    state_flag = 1;
    CCW();
  }
  if(motor_flag==1){
    STOP();
    RESET();
  }
}
void PAUSE_O(){
  if(motor_flag==2){
    state_flag = 3;
    CW();
  }
  if(motor_flag==1){
    STOP();
    RESET();
  }
}
void CLOSE(){
  if(motor_flag!=2){
    state_flag = 2;
    CCW();
    if(time_pre==0){
      time_pre = millis();
    }
    if(motor_flag==1&&num==0){
      delay(300);
      num++;
    }
  }
  else if(motor_flag==2&&num>0){
    delay(200);
    STOP();
    RESET();    
    cnt = 0;
  }
}
void OPEN(){
  if(motor_flag!=0){
    state_flag = 0;
    CW();
    if(time_pre==0){
      time_pre = millis();
    }
    if(motor_flag==1&&num==0){
      delay(300);
      num++;
    }
  }
  else if(motor_flag==0&&num>0){
    delay(200);
    STOP();
    RESET();
    cnt = 0;
  }
}

void loop() {
  if(start_num==0){
    delay(500);
    start_num++;
  }
  
  if(sw1==1&&sw2==0){       //모터 상태 정의
    motor_flag = 0;
  }
  else if(sw1==0&&sw2==0){
    motor_flag = 1;    
  }
  else if(sw1==0&&sw2==1){ 
    motor_flag = 2; 
  }

  if(time_pre!=0){            //모터 움직임 시간 측정
    time_cur = millis();
    time_res = time_cur - time_pre;
  }

  
  mag = Serial.read();                               //센서 값 받음
  if((mag>0x2F&&mag<0x3A)||(mag>0x40&&mag<0x5B)){
    mag_value = mag;
  }
  
  if(sw3==0&&motor_flag!=0){
    open_flag = 1; 
  }
  ////예외 처리////
  else if(close_bu==1){                                              //에러 이상으로 스위치를 눌러야 잠김
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(7, HIGH);
    if(open_flag==1){
      RESET();
      close_bu = 0;
      cnt = 0;
    }
  }
  else if((state_flag==2&&mag_value!='2')||err_flag==1){        //잠금상태로 가는 도중에 사용자가 문을 여는 경우
    if(state_flag==4&&cnt>2){                                   //열었다 닫았다를 반복하는 경우
      if(err_num==0){
        time_pre = millis();
        err_num++;
      }
      time_cur = millis();
      digitalWrite(5, HIGH);
      digitalWrite(6, HIGH);
      digitalWrite(7, HIGH);
         
      if(time_res>10000){   
        RESET();
        cnt = 0;
      }
    }
    else{
      if(err_num==0){                                             //(O)
        cnt++;
        err_flag = 1;
        motor_flag = 2;
      }
      err_num++;
      PAUSE_O();

      if(state_flag==4&&cnt>2){
        err_flag = 1;
      }
    }
    
      

  }
  
  else if((state_flag==0&&time_res>3000)||err_flag==2){          //시간 안에 열림상태로 못가는 경우   
    if(err_num==0){                                              //(O)
      err_flag = 2;
      motor_flag = 0;
      cnt++;
    }
    err_num++;
    PAUSE_C();
    if(time_res==0&&cnt<=2){
      open_flag = 1;
    }
    else if(time_res==0&&cnt>2){
      LED_TWINKLE();
      RESET();
      cnt = 0;
    }
  }
  else if((mag_value=='1'&&motor_flag==0&&open_flag==0)||err_flag==4){                          //문이 열림상태인데 사용자가 일정시간 이상 문을 안여는 경우
    if(err_num==0){ 
      time_pre = millis();
      err_num++;
    }
    time_cur = millis();

    if(time_res>5000){
      err_flag = 4;
      PAUSE_C();
    }
  }
  
  else if((state_flag==2&&time_res>3000)||err_flag==3){       //시간 안에 잠금상태로 못가는 경우       
    if(err_num==0){
      cnt++;
      err_flag = 3;
      motor_flag = 2;
    }
    err_num++;
    PAUSE_O();
    if(state_flag==4&&cnt>2){
      close_bu = 1;  
    }
  }

  ////기본 구동////
  else if(open_flag==1){
    OPEN();
  }
  else if(mag_value=='2'){
    open_pre = 0;
    open_cur = 0;
    CLOSE();
  }
  else if(open_pre!=0){
    open_cur = millis();
    if(mag_value!='2'&&(open_cur-open_pre)>5000){               //문을 너무 오래 열어놓은 경우 
      digitalWrite(7, HIGH);
    }
    PAUSE_C();
  }
  else if(mag_value=='0'){                                              //(O)
    open_pre = millis();
    err_num++;
  }


  
//---------------------------------//motor test
  if(mag_value=='Q'){
    PAUSE_C();
  }
  else if(mag_value=='W'){
    CLOSE();
  }
  else if(mag_value=='E'){
    OPEN();
  }
  else if(mag_value=='R'){
    PAUSE_O();
  }
  else if(mag_value=='T'){
    STOP();
  }

  else if (mag_value == 'Y') {
    CW();
  }
  else if (mag_value == 'U') {
    CCW();
  }
}
