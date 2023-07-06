#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);
// 행 : 20, 열 : 4
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
  lcd.init(); // initialize the lcd 
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0); //(커서 위치, 줄 위치)
  lcd.print("Welcome to");
  lcd.setCursor(0,1);
  lcd.print("StorHub Korea YDP");
  lcd.setCursor(0,2);
  lcd.print("");
  lcd.setCursor(0,3);
  lcd.print("Use CARD or Press *");
}

void loop()
{
}
