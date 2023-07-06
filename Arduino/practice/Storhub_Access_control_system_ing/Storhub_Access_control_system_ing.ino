/*
  Project Name : Storhub_Access_control_system
  Subtitle : Storhub
  Reporting date : 22.11.08
  Update date : 23.05.03
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

/*
 * Arduino NANO to 자기 호출 버튼(납땜)
 * GND  to  COM
 * D2   to  NO
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
 * 화면4 : 4
 * 화면4-2 : 4
 * 화면5 : 5
 * 화면6 : 6
 */

//switch1 : 눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
//switch_cnt : default 값 : 0 ; 스위치가 눌려, SendPacket() 함수를 실행하면 1
int key=0, SerFlag=0, ScreenFlag = 0, pwlength = 0, switch1 = 1, switch_cnt = 0;

//프로토콜 변수
byte stx, addr, cmd, receive_data1, receive_data2, receive_data3, receive_data4, receive_data5, receive_data6, send_data1, send_data2, send_data3, send_data4, send_data5, send_data6, etx, sum;

/*
     * wait 시, 
     * 1) 초기화면에서만 사용자의 번호 키 입력 받아 wait에서 깨어남.
     * 2) 현재 날짜, 시간 전송 받으면 값만 처리하고, wait에서는 깨어나지 않음.
     * 3) 비밀번호 검증 결과 받는 것은 wait 시가 아니므로 해당 안됨.
     * 4) LCD에서 현재 어떤 화면인지를 물어보는 요청은 wait에서 깨어나지 않음.
     * 
     * wait_1 : 초기화면 시, 사용
     * wait_2 : 초기화면이 아닐 때, 사용
*/
char user_name1, user_name2, user_name3, user_name4, user_name5, user_name6;

void wait_1(unsigned long duration_ms)
{
  unsigned long time_start = millis();
  unsigned long time_end = time_start;
  bool is_complete = false;
  while (!is_complete)
  {
    //스위치 상태 응답
    if(switch1 == 0 && switch_cnt == 0){
      SendPacket_switch();
      switch_cnt += 1;
    }
    else if(switch1 == 1 && switch_cnt == 1){
      switch_cnt -= 1;
    }
    
    RecievePacket();
    if(SerFlag == 1){
      break;
    }
    
    unsigned long time_current = millis();

    if (time_current - time_start >= duration_ms)
    {
      time_end = millis();
      is_complete = true;
    }
  }
}

void wait_2(unsigned long duration_ms)
{
  unsigned long time_start = millis();
  unsigned long time_end = time_start;
  bool is_complete = false;
  while (!is_complete)
  {
    //스위치 상태 응답
    if(switch1 == 0 && switch_cnt == 0){
      SendPacket_switch();
      switch_cnt += 1;
    }
    else if(switch1 == 1 && switch_cnt == 1){
      switch_cnt -= 1;
    }
    
    RecievePacket();
    
    if(SerFlag == 1 && RecPacket[1]==0x01 && RecPacket[2]==0x02){
      current_front_year = RecPacket[3];
      current_behind_year = RecPacket[4];
      current_month = RecPacket[5];
      current_day = RecPacket[6];
      current_hour = RecPacket[7];
      current_minute = RecPacket[8];
      Dayofweek();
      Ampm();
      Reset_protocol();
    }
    else if(SerFlag == 1 && RecPacket[1]==0x01 && RecPacket[2]==0x05 &&
      RecPacket[3]==0x00 &&
      RecPacket[4]==0x00 &&
      RecPacket[5]==0x00 &&
      RecPacket[6]==0x00 &&
      RecPacket[7]==0x00 &&
      RecPacket[8]==0x00){
      switch(ScreenFlag){
        case 0 :
          SenPacket_screen[3] = 0x10;
          break;
        case 1 :
          SenPacket_screen[3] = 0x11;
          break;
        case 2 :
          SenPacket_screen[3] = 0x12;
          break;
        case 3 :
          SenPacket_screen[3] = 0x13;
          break;
        case 4 :
          SenPacket_screen[3] = 0x14;
          break;
        case 5 :
          SenPacket_screen[3] = 0x15;
          break;
        case 6 :
          SenPacket_screen[3] = 0x16;
          break;
      }
      SendPacket_screen();
      Reset_protocol();
    }
    
    unsigned long time_current = millis();

    if (time_current - time_start >= duration_ms)
    {
      time_end = millis();
      is_complete = true;
    }
  }
}

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

//호출(스위치) 버튼 눌림 시, 패킷 전송
void SendPacket_switch(){
  SenPacket_switch[0] = 0x02;
  SenPacket_switch[1] = 0x02;
  SenPacket_switch[2] = 0x04;
  SenPacket_switch[3] = 0x00;
  SenPacket_switch[4] = 0x00;
  SenPacket_switch[5] = 0x00;
  SenPacket_switch[6] = 0x00;
  SenPacket_switch[7] = 0x00;
  SenPacket_switch[8] = 0x00;
  SenPacket_switch[9] = 0x03;
  SenPacket_switch[10] = 0x02+0x02+0x04+0x03;
  Serial.write(SenPacket_switch, 11);
}

//현재 화면 패킷 전송
void SendPacket_screen(){
  SenPacket_screen[0] = 0x02;
  SenPacket_screen[1] = 0x02;
  SenPacket_screen[2] = 0x05;
  SenPacket_screen[4] = 0x00;
  SenPacket_screen[5] = 0x00;
  SenPacket_screen[6] = 0x00;
  SenPacket_screen[7] = 0x00;
  SenPacket_screen[8] = 0x00;
  SenPacket_screen[9] = 0x03;
  SenPacket_screen[10] = 0x02+0x02+0x05+SenPacket_screen[3]+0x03;
  Serial.write(SenPacket_screen, 11);
}

//M/B에서 LCD로 넘어오는 패킷 분석
void PacketAnalyze(){
  if(RecPacket[0]==0x02&&RecPacket[1]==0x01&&
      (RecPacket[2]==0x01||RecPacket[2]==0x02||RecPacket[2]==0x03||RecPacket[2]==0x05)&&
      RecPacket[9]==0x03 &&
      RecPacket[10]==RecPacket[0]+RecPacket[1]+RecPacket[2]+RecPacket[3]+RecPacket[4]+RecPacket[5]+RecPacket[6]+RecPacket[7]+RecPacket[8]+RecPacket[9]
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
  else if(RecPacket[0]==0x02&&RecPacket[1]==0x01&&
      (RecPacket[2]==0x06||RecPacket[2]==0x07)&&
      RecPacket[9]==0x03
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
  //lcd.setCursor(2,1);
  //lcd.print("StorHub Korea SL");
  
  //논현점
  lcd.setCursor(2,1);
  lcd.print("StorHub Korea NH");
  
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
void Screen4(){
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
  if(cmd==(byte)0x06){
    lcd.setCursor(9,0);
  }
  else if(cmd==(byte)0x07){
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
  if(current_hour > 12){
    current_hour -= 12;
    ampm = "PM";
  }
  else{
    ampm = "AM";
  }
}

void Switchcheck() {
  switch1 = digitalRead(2);
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

  MsTimer2::set(50, Switchcheck); //20ms마다 스위치 상태 측정하겠다.
  MsTimer2::start();

  pinMode(2, INPUT_PULLUP); //눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  wait_1(20);
}

void loop() {
  //스위치 상태 응답
  if(switch1 == 0 && switch_cnt == 0){
    SendPacket_switch();
    switch_cnt += 1;
  }
  else if(switch1 == 1 && switch_cnt == 1){
    switch_cnt -= 1;
  }
  
  if(SerFlag==0){
    RecievePacket();
    wait_1(3000);

    //위의 wait(3000) 함수에서 SerFlag 값이 '1'로 바뀌면 Change_Screen1() 함수를 실행하지 않게 하는 조건문
    if(SerFlag==0){
      Change_Screen1();
    }
  }
  else{
    PacketAnalyze();
    
    //Screen1_1 또는 Screen1_2 에서 사용자가 '*' 입력하면, Screen2로 이동
    if((ScreenFlag==0 || ScreenFlag==1) && addr==(byte)0x01 && cmd==(byte)0x01 &&
        receive_data1==(byte)0x1A &&
        receive_data2==(byte)0x00 &&
        receive_data3==(byte)0x00 &&
        receive_data4==(byte)0x00 &&
        receive_data5==(byte)0x00 &&
        receive_data6==(byte)0x00){
      Screen2();
      Reset_protocol();
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
        case 0 :
          Screen1_1();
          break;
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
      Reset_protocol();
    }

    //Screen2 에서 사용자가 비밀번호 입력하면, LCD에 표시
    if(ScreenFlag==2 && addr==(byte)0x01 && cmd==(byte)0x01 &&
      receive_data1!=(byte)0x1A && receive_data1!=(byte)0x1B &&
      receive_data2==(byte)0x00 &&
      receive_data3==(byte)0x00 &&
      receive_data4==(byte)0x00 &&
      receive_data5==(byte)0x00 &&
      receive_data6==(byte)0x00 &&
      pwlength < 6){
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
      
      Reset_protocol();
    }

    //Screen2에서 사용자가 비밀번호 입력 후 '*' 버튼을 눌렀을 때, Screen3로 이동
    if(ScreenFlag==2 && addr==(byte)0x01 && cmd==(byte)0x01 &&
      receive_data1==(byte)0x1A &&
      receive_data2==(byte)0x00 &&
      receive_data3==(byte)0x00 &&
      receive_data4==(byte)0x00 &&
      receive_data5==(byte)0x00 &&
      receive_data6==(byte)0x00
      ){
      pwlength = 0;
      Screen3();
      Reset_protocol();
    }

    //Screen3에서 사용자가 '#' 버튼을 눌렀을 때, Screen1로 이동
    if(ScreenFlag==3 && addr==(byte)0x01 && cmd==(byte)0x01 &&
      receive_data1==(byte)0x1B &&
      receive_data2==(byte)0x00 &&
      receive_data3==(byte)0x00 &&
      receive_data4==(byte)0x00 &&
      receive_data5==(byte)0x00 &&
      receive_data6==(byte)0x00
      ){
      Screen1_1();
      Reset_protocol();
    }
    
    //Screen3에서 비밀번호 검증 결과를 받았을 때, 결과에 따라 Screen4 또는 Screen5로 이동
    if(ScreenFlag==3 && addr==(byte)0x01 && cmd==(byte)0x03 &&
      receive_data1==(byte)0x10 &&
      receive_data2==(byte)0x00 &&
      receive_data3==(byte)0x00 &&
      receive_data4==(byte)0x00 &&
      receive_data5==(byte)0x00 &&
      receive_data6==(byte)0x00
      ){
      Screen4();      
      Reset_protocol();
    }
    else if(ScreenFlag==3 && addr==(byte)0x01 && cmd==(byte)0x03 &&
      receive_data1==(byte)0x11 &&
      receive_data2==(byte)0x00 &&
      receive_data3==(byte)0x00 &&
      receive_data4==(byte)0x00 &&
      receive_data5==(byte)0x00 &&
      receive_data6==(byte)0x00
      ){
      Screen5();
      Reset_protocol();
      wait_2(2000);
      Screen6();
      Reset_protocol();
      wait_2(100);
      
      for(int i = 0 ; i < 3 ; i++){
        Screen5();
        Reset_protocol();
        wait_2(1200);
        Screen6();
        Reset_protocol();
        wait_2(100);
      }
      Screen1_1();
      Reset_protocol();
    }

    //사용자 이름 또는 사용자 휴대전화 뒷번호 4자리 전송(1)
    if(ScreenFlag==4 && addr==(byte)0x01 && cmd==(byte)0x06){
      user_name1 = receive_data1;
      user_name2 = receive_data2;
      user_name3 = receive_data3;
      user_name4 = receive_data4;
      user_name5 = receive_data5;
      user_name6 = receive_data6;
      
      Screen4_2();
      Reset_protocol();
    }
    
    //사용자 이름 또는 사용자 휴대전화 뒷번호 4자리 전송(2)
    if(ScreenFlag==4 && addr==(byte)0x01 && cmd==(byte)0x07){
      user_name1 = receive_data1;
      user_name2 = receive_data2;
      user_name3 = receive_data3;
      user_name4 = receive_data4;
      user_name5 = receive_data5;
      user_name6 = receive_data6;
      
      Screen4_2();
      wait_2(5000);
      Screen1_1();
      Reset_protocol();
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
      Reset_protocol();
    }

    //현재 LCD 화면에 대한 요청 및 응답
    if(addr==(byte)0x01 && cmd==(byte)0x05 &&
      receive_data1==(byte)0x00 &&
      receive_data2==(byte)0x00 &&
      receive_data3==(byte)0x00 &&
      receive_data4==(byte)0x00 &&
      receive_data5==(byte)0x00 &&
      receive_data6==(byte)0x00
      ){
      switch(ScreenFlag){
        case 0 :
          SenPacket_screen[3] = 0x10;
          break;
        case 1 :
          SenPacket_screen[3] = 0x11;
          break;
        case 2 :
          SenPacket_screen[3] = 0x12;
          break;
        case 3 :
          SenPacket_screen[3] = 0x13;
          break;
        case 4 :
          SenPacket_screen[3] = 0x14;
          break;
        case 5 :
          SenPacket_screen[3] = 0x15;
          break;
        case 6 :
          SenPacket_screen[3] = 0x16;
          break;
      }
      SendPacket_screen();
    }    
    Reset_protocol();
  }
}
