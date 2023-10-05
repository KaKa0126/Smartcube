/*
  Project Name : Storhub_Windbank_Switch
  Subtitle : Storhub_Windbank_Switch
  Reporting date : 23.09.14
  Update date : 23.10.05
  written by Smartcube HW Senior Staff 'HEONSEOK HA'
*/

/*
 * Relay Model : RELAY-M1(CH8), 5V
 * Arduino Model : Arduino ATmega2560
 */

#include <MsTimer2.h>
#include <Wire.h>

#define relay1 2
#define relay2 3
#define relay3 4
#define relay4 5
#define relay5 6
#define relay6 7

int switch1;

void switchcheck() {
  switch1 = digitalRead(52);
}

void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);

  //NC 연결을 베이스로 함(스토어허브 리셉션 인터컴과 연결)
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  
  Serial.begin(9600);

  MsTimer2::set(20, switchcheck); //20ms마다 스위치 상태 측정하겠다.
  MsTimer2::start();
  
  //스위치 핀으로 GND와 Digital pin '52' 사용
  //스위치 단선이면 '1', 도통이면 '0' 출력
  pinMode(52, INPUT_PULLUP);
  
  //Serial.println("Switch test");
  delay(20);  //스위치 초기 오류 없앰
}

void loop() {
  //스위치 ON, 릴레이 NO, 와인뱅크 리셉션 인터컴과 연결
  if(switch1 == 0){
    digitalWrite(relay1, LOW);
    digitalWrite(relay2, LOW);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    digitalWrite(relay5, LOW);
    digitalWrite(relay6, LOW);
  }
  //스위치 OFF, 릴레이 NC, 스토어허브 리셉션 인터컴과 연결
  else if(switch1 == 1){
    digitalWrite(relay1, HIGH);
    digitalWrite(relay2, HIGH);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay5, HIGH);
    digitalWrite(relay6, HIGH);
  }
}
