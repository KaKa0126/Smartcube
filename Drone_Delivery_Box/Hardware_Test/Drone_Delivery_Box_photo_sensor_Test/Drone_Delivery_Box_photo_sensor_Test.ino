/*
 * photo sensor : Autonics, BS5-Y2M
 * 
 * - Connect -
 * 1) Arduino GND to SMPS GND
 * 2) photo(white+blue) to SMPS GND
 * 3) photo(brown) to SMPS VCC
 * 4) photo(black) to Arduino A0(Analog pin)
 */


void setup() {  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int c = analogRead(A0);
  float a = c*5.0/1024.0;
  if(a <= 0){
    Serial.println("detected");
  }
  else{
    Serial.println("undetected");
  }
  delay(100);
}
