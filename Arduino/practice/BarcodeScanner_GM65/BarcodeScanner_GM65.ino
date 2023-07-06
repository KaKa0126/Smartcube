/*
 * 제품명 : GM65 
 * 기능 : 바코드스캐너
 * 참고 사이트 : 
 * 1) https://blog.naver.com/darknisia/220808977305
 * 2) https://bota.tistory.com/1377
 */

#include <SoftwareSerial.h>
SoftwareSerial GM65(10, 11);  //Arduino RX, TX
/*
 * GM65의 TX = 아두이노의 10번 연결
 * GM65의 RX = 아두이노의 11번 연결
 * 
 * RX로 사용 가능한 핀 번호
 * ATmega328(Uno) : all
 * ATmega2560 : 10~15, 50~53, 62~69
 * ATmega32u4(Leonardo) : 8~11, 14~16
 */

void setup() {
  Serial.begin(115200);// PC-아두이노간 통신
  GM65.begin(115200); //GM65-아두이노간 통신
}

void loop() {
  if(GM65.available()){
    String barcode = GM65.readString();
    Serial.println(barcode);
  }
}
