#include <EEPROM.h>
byte x = 0x10;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  //x = EEPROM.read(0);
  y = (int)x;
  EEPROM.write(0,y);
  EEPROM.write(1,y);
  EEPROM.write(2,y);
}

void loop() {
  // put your main code here, to run repeatedly:
}
