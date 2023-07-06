#include <MsTimer2.h>
#include <Wire.h>

int error_cnt = 1;
byte addr, addr_past;

void Error_check(){
  if(addr != addr_past || addr == 0){
    error_cnt = 0;
  }
  else{
    error_cnt = 5;
  }
  addr_past = addr;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  //1[sec]마다 Error_check 함수를 실행하여, 하드웨어 고장 또는 오류나면 모터 동작 멈추게 함
  MsTimer2::set(1000, Error_check);
  MsTimer2::start();
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(error_cnt);
  delay(3000);
  Serial.println(error_cnt);
  addr = (byte)0x20;
  delay(10000);
  Serial.println(error_cnt);
  addr = (byte)0x00;
  delay(2000);
  Serial.println(error_cnt);
}
