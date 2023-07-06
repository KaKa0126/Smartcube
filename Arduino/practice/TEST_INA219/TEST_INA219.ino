#include <Adafruit_INA219.h>
#include <Wire.h>

float max_current = 0;
float min_current = 5;

Adafruit_INA219 ina219;

void setup() {
  Serial.begin(115200);
  uint32_t currentFrequency;
  ina219.begin();
}

void Reset() {
  max_current = 0;
  min_current = 5;
}

void loop() {
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_A = 0;
  float loadvoltage = 0;
  float power_mW = 0;
  char input;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_A = ina219.getCurrent_mA()/1000;
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage/ 1000);

  if(current_A > max_current){
    max_current = current_A;
  }
  
  if(current_A < min_current){
    min_current = current_A;
  }
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'Q') {
      Reset();
    }
  }

  //Serial.print("Bus Voltage: "); Serial.print(busvoltage); Serial.println(" V");
  //Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  //Serial.print("Load Vlotage: "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current: "); Serial.print(current_A); Serial.println(" A");
  Serial.print("Max Current: "); Serial.print(max_current); Serial.println(" A");
  Serial.print("Min Current: "); Serial.print(min_current); Serial.println(" A");
  //Serial.print("Power: "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");
  
  
  delay(20);  
}
