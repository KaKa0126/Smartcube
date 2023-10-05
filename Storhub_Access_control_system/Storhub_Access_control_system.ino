/*
  Project Name : Storhub_Access_control_system
  Subtitle : Storhub
  Reporting date : 22.11.08
  Update date : 23.10.04
  written by Smartcube HW Staff 'HEONSEOK HA'
*/

#include <MsTimer2.h>
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

//현재 날짜, 시간, 요일, ampm  변수(데이터 업데이트 전 임의의 값 출력)
byte current_front_year=0x14, current_behind_year=0x16, current_month=0x0A, current_day=0x0B, current_hour=0x11, current_minute=0x2C;
String dayofweek, ampm;

//RecPacket[] : 수신된 프로토콜 배열
byte RecPacket[11]={0, }, SenPacket_switch[11]={0, }, SenPacket_screen[11]={0, };

//key : 수신된 프로토콜 배열 내의 갯수
//SerFlag : 시리얼통신 중이면 '1'; 시리얼 통신 중이 아니면 '0'
/*
 * ScreenFlag : 현재 LCD 창에 출력되는 화면
 * - ScreenFlag 값 -
 * 화면1-1 : 0
 * 화면1-2 : 1
 * 화면2 : 2
 * 화면3 : 3
 * 화면4-1 : 4
 * 화면4-2 : 4
 * 화면5 : 5
 * 화면6 : 6
 */

int key=0, SerFlag=0, ScreenFlag = 0, pwlength = 0;

//프로토콜 변수
byte stx, addr, cmd, receive_data1, receive_data2, receive_data3, receive_data4, receive_data5, receive_data6, send_data1, send_data2, send_data3, send_data4, send_data5, send_data6, etx, sum;
char user_name1, user_name2, user_name3, user_name4, user_name5, user_name6;

void Reset_protocol(){
  for(int i=0; i<11; i++){
    RecPacket[i] = 0;
  }
  key=0;
  SerFlag=0;
  stx=0;
  addr=0;
  cmd=0;
  receive_data1=0;
  receive_data2=0;
  receive_data3=0;
  receive_data4=0;
  receive_data5=0;
  receive_data6=0;
  user_name1 = 0x10;
  user_name2 = 0x10;
  user_name3 = 0x10;
  user_name4 = 0x10;
  user_name5 = 0x10;
  user_name6 = 0x10;
  etx=0;
  sum=0;
}

//패킷 수신
void RecievePacket(){
  if(Serial.available()<=0)
    return;

  char ch = Serial.read();
  if(ch==(byte)0x02||RecPacket[0]==(byte)0x02){
    RecPacket[key] = ch;
    key++;
  }
  if(key==11){
    SerFlag = 1;
    key = 0;
  }
}

//M/B에서 LCD로 넘어오는 패킷 분석
void PacketAnalyze(){
  if(RecPacket[0]==0x02&&RecPacket[1]==0x01&&
      (RecPacket[2]==0x01||RecPacket[2]==0x03||RecPacket[2]==0x04||RecPacket[2]==0x05)&&
      RecPacket[9]==0x03 &&
      RecPacket[10]==(RecPacket[0]+RecPacket[1]+RecPacket[2]+RecPacket[3]+RecPacket[4]+RecPacket[5]+RecPacket[6]+RecPacket[7]+RecPacket[8]+RecPacket[9])%256
    )
  {
    stx = RecPacket[0];
    addr = RecPacket[1];
    cmd = RecPacket[2];
    receive_data1 = RecPacket[3];
    receive_data2 = RecPacket[4];
    receive_data3 = RecPacket[5];
    receive_data4 = RecPacket[6];
    receive_data5 = RecPacket[7];
    receive_data6 = RecPacket[8];
    etx = RecPacket[9];
    sum = RecPacket[10];
  }
  else if((RecPacket[0]==0x02)&&(RecPacket[1]==0x01)&&
      (RecPacket[2]==0x02)&&
      (RecPacket[9]==0x03)&&
      (RecPacket[10]==(RecPacket[0]+RecPacket[1]+RecPacket[2]+RecPacket[3]+RecPacket[4]+RecPacket[5]+RecPacket[6]+RecPacket[7]+RecPacket[8]+RecPacket[9])%256)&&
      (RecPacket[3] == 20) &&
      (RecPacket[4] >= 23) &&
      (RecPacket[4] < 73) &&
      (RecPacket[5] >= 1) &&
      (RecPacket[5] <= 12) &&
      (RecPacket[6] >= 1) &&
      (RecPacket[6] <= 31) &&
      (RecPacket[7] >= 0) &&
      (RecPacket[7] <= 23) &&
      (RecPacket[8] >= 0) &&
      (RecPacket[8] <= 59)
    )
  {
    stx = RecPacket[0];
    addr = RecPacket[1];
    cmd = RecPacket[2];
    receive_data1 = RecPacket[3];
    receive_data2 = RecPacket[4];
    receive_data3 = RecPacket[5];
    receive_data4 = RecPacket[6];
    receive_data5 = RecPacket[7];
    receive_data6 = RecPacket[8];    
    etx = RecPacket[9];
    sum = RecPacket[10];
  }
  else{
    Reset_protocol();
    return;
  }
}

//lcd 초기화 함수. lcd.init()는 실시간 시리얼 통신에 지장을 줘서 사용 못함
void Lcd_init(){  
  lcd.setCursor(0,0); //(커서 위치, 줄 위치)
  lcd.print("                    ");
  lcd.setCursor(0,1);
  lcd.print("                    ");
  lcd.setCursor(0,2);
  lcd.print("                    ");
  lcd.setCursor(0,3);
  lcd.print("                    ");
}

//화면1-1
void Screen1_1(){
  Lcd_init();  
  lcd.setCursor(5,0); //(커서 위치, 줄 위치)
  lcd.print("Welcome to");
  
  //마곡점
  //lcd.setCursor(2,1);
  //lcd.print("StorHub Korea MG");
  
  //선릉점
  lcd.setCursor(2,1);
  lcd.print("StorHub Korea SL");
  
  //논현점
  //lcd.setCursor(2,1);
  //lcd.print("StorHub Korea NH");
  
  //논현 와인뱅크
  //lcd.setCursor(1,1);
  //lcd.print("Winebanc Nonhyeon");
    
  lcd.setCursor(3,2);
  lcd.print("Use Mobile Web");
  lcd.setCursor(5,3);
  lcd.print("or Press *");
  ScreenFlag = 0;
}

//화면1-2
void Screen1_2(){
  Lcd_init();  
  lcd.setCursor(0,0);
  lcd.print(dayofweek);
  lcd.setCursor(12,0);
  lcd.print(current_behind_year, DEC);  //2100년 일 경우, 00이 제대로 출력되는 지 확인 필요.
  lcd.setCursor(14,0);
  lcd.print("/");
  
  if(current_month<10){
    lcd.setCursor(16,0);
  }
  else{
    lcd.setCursor(15,0);
  }
  lcd.print(current_month, DEC);
  
  lcd.setCursor(17,0);
  lcd.print("/");

  if(current_day<10){
    lcd.setCursor(19,0);
  }
  else{
    lcd.setCursor(18,0);
  }
  lcd.print(current_day, DEC);

  if(current_hour<10){
    lcd.setCursor(7,1);
  }
  else{
    lcd.setCursor(6,1);
  }
  lcd.print(current_hour, DEC);
  
  lcd.setCursor(8,1);
  lcd.print(":");

  if(current_minute<10){
    lcd.setCursor(10,1);
  }
  else{
    lcd.setCursor(9,1);
  }
  lcd.print(current_minute, DEC);

  lcd.setCursor(12,1);
  lcd.print(ampm);
  lcd.setCursor(3,2);
  lcd.print("Use Mobile Web");
  lcd.setCursor(5,3);
  lcd.print("or Press *");
  ScreenFlag = 1;
}

//화면2
void Screen2(){
  Lcd_init();
  lcd.setCursor(0,0);
  lcd.print("*   PLEASE ENTER   *");
  lcd.setCursor(2,1);
  lcd.print("YOUR ACCESS CODE");
  lcd.setCursor(4,2);
  lcd.print("_ _ _ _ _ _");
  lcd.setCursor(1,3);
  lcd.print("PRESS * WHEN DONE");
  ScreenFlag = 2;
}

//화면3
void Screen3(){
  Lcd_init();
  lcd.setCursor(0,0);
  lcd.print("*   PLEASE WAIT    *");
  lcd.setCursor(0,1);
  lcd.print("");
  lcd.setCursor(2,2);
  lcd.print("VERIFYING ACCESS");
  lcd.setCursor(0,3);
  lcd.print("");
  ScreenFlag = 3;
}

//화면4
void Screen4_1(){
  Lcd_init();
  lcd.setCursor(0,0);
  //lcd.print("Welcome, Guest");
  lcd.print("Welcome, ");
  //'Guest' 대신 '사용자 영문 이름' 또는'사용자 휴대전화 뒷번호 4자리'로 소프트웨어로 부터 받을 예정  
  lcd.setCursor(0,1);
  lcd.print("Entry is Granted");
  lcd.setCursor(0,2);
  lcd.print("Have a");
  lcd.setCursor(0,3);
  lcd.print("Nice Day!");
  ScreenFlag = 4;
}

void Screen4_2(){
  //아래 함수에서 위치를 제대로 못잡고 ex)lcd.setCursor(9,0);, 엉뚱한 출력됨
  if(cmd==(byte)0x04){
    lcd.setCursor(9,0);
  }
  else if(cmd==(byte)0x05){
    lcd.setCursor(15,0);
  }
  lcd.print(user_name1);
  lcd.print(user_name2);
  lcd.print(user_name3);
  lcd.print(user_name4);
  lcd.print(user_name5);
  lcd.print(user_name6);
  ScreenFlag = 4;
}

//화면5
void Screen5(){
  Lcd_init();
  lcd.setCursor(0,0);
  lcd.print("Access Denied -");
  lcd.setCursor(0,1);
  lcd.print("User Code is Unknown");
  lcd.setCursor(0,2);
  lcd.print("Please Contact Your");
  lcd.setCursor(0,3);
  lcd.print("Site Mgr for Help");
  ScreenFlag = 5;
}

//화면6
void Screen6(){
  Lcd_init();
  ScreenFlag = 6;
}

//대기상태일 때, 화면1-1, 화면1-2를 바꿔주는 함수
void Change_Screen1(){
  if(ScreenFlag == 0){
    Screen1_2();
  }
  else if(ScreenFlag == 1){
    Screen1_1();
  }
}

//현재 년,월,일을 입력받아 요일을 출력하는 함수
//https://en.wikipedia.org/wiki/Determination_of_the_day_of_the_week
void Dayofweek()
{
    int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
    if ( current_month < 3 )
    {
        current_behind_year -= 1;
    }      
    int calc = (current_behind_year + current_behind_year/4 - current_behind_year/100 + current_behind_year/400 + t[current_month-1] + current_day) % 7;

    switch(calc){
    case 0 :
      dayofweek = "SUNDAY";
      break;
    case 1 :
      dayofweek = "MONDAY";
      break;
    case 2 :
      dayofweek = "TUESDAY";
      break;
    case 3 :
      dayofweek = "WEDNESDAY";
      break;
    case 4 :
      dayofweek = "THURSDAY";
      break;
    case 5 :
      dayofweek = "FRIDAY";
      break;
    case 6 :
      dayofweek = "SATURDAY";
      break;
    }
}

void Ampm(){
  if(13 <= current_hour){
    current_hour -= 12;
    ampm = "PM";
  }
  else if(current_hour <= 11){
    if(current_hour == 0){
      current_hour += 12;
    }
    ampm = "AM";
  }
}

void setup() {
  //라이브러리 버전이 낮을 경우에는 lcd.init()이 아닌 lcd.begin()이 사용될 수 있다.
  //I2C 통신 초기화가 필요한 경우가 있으므로, 필수 포함되어야할 코드이다.
  lcd.init();
  lcd.backlight();
  
  Dayofweek();
  Ampm();
  Screen1_1();

  Serial.begin(9600);
  
  delay(20);
}

void loop() {  
  if(SerFlag==0){
    RecievePacket();
  }
  else{
    PacketAnalyze();
    
    //Screen Change
    if(addr==(byte)0x01 && cmd==(byte)0x03){
      switch(receive_data1){
        case (byte)0x10 :
          Screen1_1();
          break;
        case (byte)0x11 :
          Screen1_2();
          break;
        case (byte)0x12 :
          Screen2();
          break;
        case (byte)0x13 :
          pwlength = 0;
          Screen3();
          break;
        case (byte)0x14 :
          Screen4_1();
          break;
        case (byte)0x15 :
          Screen5();
          break;
        case (byte)0x16 :
          Screen6();
          break;
      }
    }
    
    //Screen2 에서 사용자가 '#' 버튼 입력하면, Screen1로 이동
    if(ScreenFlag==2 && addr==(byte)0x01 && cmd==(byte)0x01 &&
      receive_data1==(byte)0x1B &&
      receive_data2==(byte)0x00 &&
      receive_data3==(byte)0x00 &&
      receive_data4==(byte)0x00 &&
      receive_data5==(byte)0x00 &&
      receive_data6==(byte)0x00){    
      switch(pwlength){
        case 1 :
          lcd.setCursor(4,2);
          lcd.print("_");
          break;
        case 2 :
          lcd.setCursor(6,2);
          lcd.print("_");
          break;
        case 3 :
          lcd.setCursor(8,2);
          lcd.print("_");
          break;
        case 4 :
          lcd.setCursor(10,2);
          lcd.print("_");
          break;
        case 5 :
          lcd.setCursor(12,2);
          lcd.print("_");
          break;
        case 6 :
          lcd.setCursor(14,2);
          lcd.print("_");
          break;
      }
      
      if(pwlength != 0){
        pwlength -= 1;
      }
    }

    //Screen2 에서 사용자가 비밀번호 입력하면, LCD에 표시
    if(ScreenFlag==2 && addr==(byte)0x01 && cmd==(byte)0x01 &&
      receive_data1!=(byte)0x1A && receive_data1!=(byte)0x1B &&
      receive_data2==(byte)0x00 &&
      receive_data3==(byte)0x00 &&
      receive_data4==(byte)0x00 &&
      receive_data5==(byte)0x00 &&
      receive_data6==(byte)0x00){
      if(pwlength < 6){
        pwlength += 1;      
        switch(pwlength){
          case 1 :
            lcd.setCursor(4,2);
            break;
          case 2 :
            lcd.setCursor(6,2);
            break;
          case 3 :
            lcd.setCursor(8,2);
            break;
          case 4 :
            lcd.setCursor(10,2);
            break;
          case 5 :
            lcd.setCursor(12,2);
            break;
          case 6 :
            lcd.setCursor(14,2);
            break;
        }
        lcd.print(receive_data1-16, DEC);
        //아래의 코드는 Ver 3
        //lcd.print("*");        
      }
    }

    //사용자 이름 또는 사용자 휴대전화 뒷번호 4자리 전송(1)
    if(ScreenFlag==4 && addr==(byte)0x01 && cmd==(byte)0x04){
      user_name1 = receive_data1;
      user_name2 = receive_data2;
      user_name3 = receive_data3;
      user_name4 = receive_data4;
      user_name5 = receive_data5;
      user_name6 = receive_data6;
      
      Screen4_2();
    }
    
    //사용자 이름 또는 사용자 휴대전화 뒷번호 4자리 전송(2)
    if(ScreenFlag==4 && addr==(byte)0x01 && cmd==(byte)0x05){
      user_name1 = receive_data1;
      user_name2 = receive_data2;
      user_name3 = receive_data3;
      user_name4 = receive_data4;
      user_name5 = receive_data5;
      user_name6 = receive_data6;
      
      Screen4_2();
    }    

    //날짜, 시간, 요일 업데이트
    if(addr==(byte)0x01 && cmd==(byte)0x02){
      current_front_year = receive_data1;
      current_behind_year = receive_data2;
      current_month = receive_data3;
      current_day = receive_data4;
      current_hour = receive_data5;
      current_minute = receive_data6;
      
      Dayofweek();
      Ampm();
    }
    Reset_protocol();
  }
}
