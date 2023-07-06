#include <MsTimer2.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);

//띄어쓰기 값은 0x10;
char user_name0 = 0x79;
char user_name1 = 0x7A;
char user_name2 = 0x71;
char user_name3 = 0x72;
char user_name4 = 0x73;
char user_name5 = 0x74;
char user_name6 = 0x75;
char user_name7 = 0x76;
char user_name8 = 0x77;
char user_name9 = 0x78;

//lcd 초기화 함수. lcd.init()는 실시간 시리얼 통신에 지장을 줘서 사용 못함
void Lcd_init() {
  lcd.setCursor(0, 0); //(커서 위치, 줄 위치)
  lcd.print("                    ");
  lcd.setCursor(0, 1);
  lcd.print("                    ");
  lcd.setCursor(0, 2);
  lcd.print("                    ");
  lcd.setCursor(0, 3);
  lcd.print("                    ");
}

void Screen() {
  Lcd_init();
  lcd.setCursor(0, 0);
  lcd.print("*   PLEASE ENTER   *");
  lcd.setCursor(2, 1);
  lcd.print("YOUR ACCESS CODE");
  lcd.setCursor(0, 2);
  lcd.print(user_name0);
  lcd.print(user_name1);
  lcd.print(user_name2);
  lcd.print(user_name3);
  lcd.print(user_name4);
  lcd.print(user_name5);
  lcd.print(user_name6);
  lcd.print(user_name7);
  lcd.print(user_name8);
  lcd.print(user_name9);
  lcd.setCursor(1, 3);
  lcd.print("PRESS * WHEN DONE");
}

void setup() {
  //라이브러리 버전이 낮을 경우에는 lcd.init()이 아닌 lcd.begin()이 사용될 수 있다.
  //I2C 통신 초기화가 필요한 경우가 있으므로, 필수 포함되어야할 코드이다.
  lcd.init();
  lcd.backlight();

  Screen();

  Serial.begin(9600);
}

void loop() {

}
