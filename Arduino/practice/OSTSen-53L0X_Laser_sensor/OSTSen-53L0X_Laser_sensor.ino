#include "Adafruit_VL53L0X.h"
 
Adafruit_VL53L0X TOF = Adafruit_VL53L0X();
 
void setup() {
  Serial.begin(9600);
  Serial.println("VL53L0X test");
 
  // 일반적으로는 I2C 주소와 디버그 모드 설정값을 넘겨줘서 시작.
  // boolean Adafruit_VL53L0X::begin(uint8_t i2c_addr, boolean debug ) 
  
  if (!TOF.begin()) { // VL53L0X 기본 I2C 주소:0x29, 디버그 모드:false로 센서 준비.
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
}
 
 
void loop() {
  VL53L0X_RangingMeasurementData_t measure; // 측정값을 담을 구조체 변수
    
  Serial.print("Reading a measurement... ");
  
  TOF.rangingTest(&measure, false); // true를 주면 디버그용 데이터를 받아옴
 
  // 이번 측정의 상태값. 장치 의존적인 값. 일반적으로 4면 에러. 0이면 측정값이 정상임을 나타냄.
  if(measure.RangeStatus != 4) {  
    Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  } 
  else {
    Serial.println(" out of range ");
  }
    
  delay(1000);
}
