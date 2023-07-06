#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F,16,2);
LiquidCrystal_I2C lcd2(0x3E,16,2);
// 행 : 16, 열 : 2
// set the LCD address to 0x27 for a 16 chars and 2 line display
/*
 * https://playground.arduino.cc/Main/I2cScanner/
 * 위 사이트에서 LCD address 확인 가능
 * 
 * 안될 시, 참고 링크 : https://eduino.kr/product/detail.html?product_no=133
 * 
 * Arduino NANO to LCD_1602
 * A4 to SDA
 * A5 to SCL
 * 5V to VCC
 * GND to GND
*/

void setup()
{
  lcd.init();
  lcd2.init();
  lcd.backlight();
  lcd2.backlight();
  
  lcd.setCursor(0,0); //(커서 위치, 줄 위치)
  lcd.print("20,000");
  lcd2.setCursor(0,0); //(커서 위치, 줄 위치)
  lcd2.print("10,000");
}

void loop()
{
}
