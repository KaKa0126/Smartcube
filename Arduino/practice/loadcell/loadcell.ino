/*
* 엘레파츠 전자저울 예제 프로그램 - loadcell + HX711
* 사용 라이브러리 : https://github.com/bogde/HX711
* 
* HX711 board / Arduino
* VCC       to  5V
* GND       to  GND
* Dout(DT) PIN  to  D2
* SCK PIN   to  D3
* 
* 로드셀 / HX711 board
* RED   to E+(== 5V)
* BLACK to E-(== GND)
* WHITE to A-
* GREEN to A+
* 
*/

#include "HX711.h"

// HX711 circuit wiring
#define LOADCELL_DOUT_PIN         2
#define LOADCELL_SCK_PIN          3
#define LOADCELL2_DOUT_PIN         4
#define LOADCELL2_SCK_PIN          5

// scale - 10Kg loadcell : 226 / 5kg loadcell : 470
// ADC 모듈에서 측정된 결과값을 (loadcellValue)값 당 1g으로 변환해 줌
float loadcellValue = 470.0;

HX711 scale;
HX711 scale2;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);

  // 로드셀 HX711 보드 pin 설정
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale2.begin(LOADCELL2_DOUT_PIN, LOADCELL2_SCK_PIN);

  // 부팅 후 잠시 대기 (2초)
  delay(2000);

  // 측정값 1회 읽어오기
  Serial.print("read: \t\t\t");
  Serial.println(scale.read());
  Serial.print("read2: \t\t\t");
  Serial.println(scale2.read());

  delay(1000);

  // 스케일 설정
  scale.set_scale(loadcellValue);
  scale2.set_scale(loadcellValue);
  
  // 오프셋 설정(10회 측정 후 평균값 적용) - 저울 위에 아무것도 없는 상태를 0g으로 정하는 기준점 설정(저울 위에 아무것도 올려두지 않은 상태여야 합니다.)   
  scale.tare(10);
  scale2.tare(10); 

  // 설정된 오프셋 및 스케일 값 확인
  Serial.print("Offset value :\t\t");
  Serial.println(scale.get_offset());
  Serial.print("Scale value :\t\t");
  Serial.println(scale.get_scale());
  // (read - offset) 값 확인 (scale 미적용)
  Serial.print("(read - offset) value: \t");
  Serial.println(scale.get_value());
  
  delay(2000);

  Serial.print("Offset value2 :\t\t");
  Serial.println(scale2.get_offset());  
  Serial.print("Scale value2 :\t\t");
  Serial.println(scale2.get_scale());
  // (read - offset) 값 확인 (scale 미적용)
  Serial.print("(read - offset) value2: \t");  
  Serial.println(scale2.get_value());
  
  delay(2000);
}

void loop() {
  // put your main code here, to run repeatedly:

  // 오프셋 및 스케일이 적용된 측정값 출력 (5회 측정 평균값) 
  Serial.print("value :\t");
  Serial.print(scale.get_units(5), 2);    // 5회 측정 평균값, 소수점 아래 2자리 출력
  Serial.println(" g");
  Serial.print("value2 :\t");
  Serial.print(scale2.get_units(5), 2);    // 5회 측정 평균값, 소수점 아래 2자리 출력
  Serial.println(" g");

  // 1초 대기
  delay(1000);  
}
