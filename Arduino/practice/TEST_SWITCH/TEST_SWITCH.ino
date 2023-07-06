#include <MsTimer2.h>
#include <Wire.h>

int switch1;

void setup() {
  Serial.begin(9600);

  MsTimer2::set(20, switchcheck); //20ms마다 스위치 상태 측정하겠다.
  MsTimer2::start();

  pinMode(2, INPUT_PULLUP); //눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  
  Serial.println("스위치 테스트");
  delay(20);  //스위치 초기 오류 없앰
}

void switchcheck() {
  switch1 = digitalRead(2);
}

void loop() {  
  if(switch1 == 0){
    Serial.println("스위치 눌림");
  }
  else if(switch1 == 1){
    Serial.print("");
  }
}
