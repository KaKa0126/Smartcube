/*
 * 빨간색 선 : 12V
 * 검정색 선 : GND
 * 파란색 선 : RX(읽기전용 제품이므로 사용 불가)
 * 노란색 선 : TX
 * 
 * 사용법
 * 1. 아래의 코드를 아두이노에 업로드 한다.
 * 2. 제품에 전원을 인가한 후, 노란색 선을 아두이노의 RX와 연결한다. 
 * 3. 시리얼 모니터를 띄운다.
 * 4. RFID 카드를 제품에 가까이 댄다.
 * 5. 시리얼 모니터에 출력된 값을 확인한다.
 */

#include <SoftwareSerial.h>
SoftwareSerial RFID_102A(10, 11);  //Arduino RX, TX

byte barcode[30];

void setup(){
  Serial.begin(9600);
  RFID_102A.begin(9600);
}

void loop(){
  String Read = "";
  
  if(RFID_102A.available()){
    delay(20);  //시리얼 통신에 들어온 전부를 받을 때 까지의 대기 시간 넣어줘야 함.
    for(int i = 0 ; i < 30 ; i++){
      barcode[i] = RFID_102A.read();  //barcode 배열에 int형으로 저장 됨.
      if(barcode[i] != 255) // 쓰레기 값 byte 제외
      {
        Read += String(barcode[i], HEX);
      }
    }    
    Serial.println(Read);
  }
}
