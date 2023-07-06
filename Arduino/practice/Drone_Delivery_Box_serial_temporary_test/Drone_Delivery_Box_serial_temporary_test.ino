//2022.11.01 : Hatch velocity slow code update
//2022.11.22 : protocol update
//2022.11.29 : protocol update_EEPROM + MsTimer2
//lift 1층으로 가고 delay(600) 맞는지 확인(현장 코드와 비교)
#include <EEPROM.h>
#include <MsTimer2.h>
#include <Wire.h>
/*
 * EEPROM 사용 시, 참고 사항
 * EEPROM의 각 address에 저장될 값의 초기 값 저장이 필요한 경우, 아래와 같은 2가지 방법이 있다.
 *  1) 소프트웨어에서 아두이노로 State Reset 프로토콜 송신
 *  2) setup()함수 내의 State_read()함수를 실행하지 않고 펌웨어 업로드
 */

#define VEL1 2
#define VEL2 3
#define VEL3 4
#define VEL4 5
#define VEL5 6
#define VEL6 7
#define VEL7 8
#define VEL8 9
#define RD1 22
#define LS1 23
#define RD2 24
#define LS2 25
#define RD3 26
#define LS3 27
#define RD4 28
#define LS4 29
#define RD5 30
#define LS5 31
#define RD6 32
#define LS6 33
#define RD7 34
#define LS7 35
#define RD8 36
#define LS8 37

const int Photo_0=13,
          Photo_1=12,
          Photo_2=11,
          Photo_3=10,
          Photo_4=38,
          Photo_5=39,
          Photo_6=40,
          Photo_7=41,
          Photo_8=42,
          Photo_9=43,
          Photo_10=44,
          Photo_11=45,
          Photo_12=46,
          Photo_13=47,
          Photo_14=48,
          Photo_15=49, 
          Photo_16=50,
          Photo_17=51,
          Photo_18=52,
          Photo_19=53;
          
// 포토센서 번호로 변환하는 함수 내에서 사용되는 배열         
int temp_val[20]={0,}, val[20]={0,}, photo_flag[20]={0,}, photo_cnt[4]={0,};
// 반복 방지용 플래그
int Hatch_flag[2]={0,}, Arrange_flag[4]={0,};
// 하드웨어 고장 또는 오류 관련 변수
int error_cnt = 0;
byte addr_past;
int hatch_max_time = 15, around_bar_max_time = 15, lift_max_time = 50, tilting_max_time = 15;

//RecPacket[] : 수신된 프로토콜 배열
//SendPacket[] : 송부할 프로토콜 배열
byte RecPacket[9]={0, }, SendPacket[9]={0, };

//key : 수신된 프로토콜 배열 내의 갯수
//SerFlag : 시리얼통신 중이면 1, 아니면 0
int key=0, SerFlag=0;

//프로토콜 변수
byte stx, addr, cmd, data1, data2, data3, data4, etx, sum;
//상태 데이터 변수, EEPROM의 각 address(0, 1, 2)에 저장될 초기 값 저장
byte state_data1 = 0x10, state_data2 = 0x10, state_data3 = 0x10;

void Reset_protocol(){
  for(int i=0; i<9; i++){
    RecPacket[i] = 0;
    SendPacket[i] = 0;
  }
  key=0;
  SerFlag=0;
  stx=0;
  addr=0;
  cmd=0;
  data1=0;
  data2=0;
  data3=0;
  data4=0;
  etx=0;
  sum=0;
}

// 모터
void Motor_CW(int num){
  digitalWrite((num+1), HIGH);
  digitalWrite((20+num*2), LOW);
  digitalWrite((21+num*2), LOW);
}

void Motor_CCW(int num){
  digitalWrite((num+1), HIGH);
  digitalWrite((20+num*2), HIGH);
  digitalWrite((21+num*2), LOW);
}

void Motor_OFF(int num){
  digitalWrite((num+1), LOW);
  digitalWrite((20+num*2), HIGH);
  digitalWrite((21+num*2), HIGH);
}

void Motor_Slow_CW(int num){
  analogWrite((num+1), 100);
  digitalWrite((20+num*2), LOW);
  digitalWrite((21+num*2), LOW);
}

void Motor_Slow_CCW(int num){
  analogWrite((num+1), 100);
  digitalWrite((20+num*2), HIGH);
  digitalWrite((21+num*2), LOW);
}

void Motor_Slow2_CW(int num){
  analogWrite((num+1), 200);
  digitalWrite((20+num*2), LOW);
  digitalWrite((21+num*2), LOW);
}

void Motor_Slow2_CCW(int num){
  analogWrite((num+1), 200);
  digitalWrite((20+num*2), HIGH);
  digitalWrite((21+num*2), LOW);
}

// 포토센서 번호로 변환하는 함수
int Check_Digital_0(int i){
  
  int num = 0;  
  if(i==10)
    num = 3;
  else if(i==11)
    num = 2;
  else if(i==12)
    num = 1;
  else if(i==13)
    num = 0;
  else
    num = i-34;
  val[num] = digitalRead(i);

  if((temp_val[num]!=val[num])&&(val[num]==0)){
    photo_cnt[0]++;
  }
  temp_val[num] = val[num];

  return photo_cnt[0];
}

int Check_Digital_1(int i){
  
  int num = 0;  
  if(i==10)
    num = 3;
  else if(i==11)
    num = 2;
  else if(i==12)
    num = 1;
  else if(i==13)
    num = 0;
  else
    num = i-34;
  val[num] = digitalRead(i);

  if((temp_val[num]!=val[num])&&(val[num]==0)){
    photo_cnt[1]++;
  }
  temp_val[num] = val[num];

  return photo_cnt[1];
}

int Check_Digital_2(int i){
  
  int num = 0;  
  if(i==10)
    num = 3;
  else if(i==11)
    num = 2;
  else if(i==12)
    num = 1;
  else if(i==13)
    num = 0;
  else
    num = i-34;
  val[num] = digitalRead(i);

  if((temp_val[num]!=val[num])&&(val[num]==0)){
    photo_cnt[2]++;
  }
  temp_val[num] = val[num];

  return photo_cnt[2];
}

int Check_Digital_3(int i){
  
  int num = 0;  
  if(i==10)
    num = 3;
  else if(i==11)
    num = 2;
  else if(i==12)
    num = 1;
  else if(i==13)
    num = 0;
  else
    num = i-34;
  val[num] = digitalRead(i);

  if((temp_val[num]!=val[num])&&(val[num]==0)){
    photo_cnt[3]++;
  }
  temp_val[num] = val[num];

  return photo_cnt[3];
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
  if(key==9){
    SerFlag = 1;
    key = 0;
  }
}

//패킷 분석
void PacketAnalyze(){
  if(RecPacket[0]==0x02&&
      (RecPacket[1]==0x08||RecPacket[1]==0x04||RecPacket[1]==0x02||RecPacket[1]==0x01||RecPacket[1]==0x10||RecPacket[1]==0x20)&&
      (RecPacket[2]==0x50||RecPacket[2]==0x60)&&
      RecPacket[7]==0x03&&
      RecPacket[8]==RecPacket[0]+RecPacket[1]+RecPacket[2]+RecPacket[3]+RecPacket[4]+RecPacket[5]+RecPacket[6]+RecPacket[7])
  {
    stx = RecPacket[0];
    addr = RecPacket[1];
    cmd = RecPacket[2];
    data1 = RecPacket[3];
    data2 = RecPacket[4];
    data3 = RecPacket[5];
    data4 = RecPacket[6];
    etx = RecPacket[7];
    sum = RecPacket[8];
  } 
  else{
    Reset_protocol();
    return;
  }
}

//패킷 송부
void SenPacket(){
  SendPacket[0] = stx;
  SendPacket[1] = addr;
  SendPacket[2] = cmd;
  
  //구동
  if(cmd == (byte)0x51){
    SendPacket[3] = data1;
    SendPacket[4] = data2;
    SendPacket[5] = data3;
  }
  //구동 시간 초과(Error) 또는 전체 상태 확인
  else if(cmd == (byte)0x52 || cmd == (byte)0x61){
    SendPacket[3] = state_data1;
    SendPacket[4] = state_data2;
    SendPacket[5] = state_data3;
  }
  
  SendPacket[6] = data4;
  SendPacket[7] = etx;
  
  if(cmd == (byte)0x51){
    SendPacket[8] = stx+addr+cmd+data1+data2+data3+data4+etx;
  }
  else if(cmd == (byte)0x52 || cmd == (byte)0x61){
    SendPacket[8] = stx+addr+cmd+state_data1+state_data2+state_data3+data4+etx;
  }

  if(stx != 0){
    Serial.write(SendPacket, 9);
  }
}

void State_save(){
  EEPROM.update(0,(int)state_data1);
  EEPROM.update(1,(int)state_data2);
  EEPROM.update(2,(int)state_data3);
}

void State_read(){
  state_data1 = (int)EEPROM.read(0);
  state_data2 = (int)EEPROM.read(1);
  state_data3 = (int)EEPROM.read(2);
}

void Error_check(){  
  if(addr != addr_past || addr == 0){
    error_cnt = 0;
  }
  switch(addr){
    case (byte)0x08:  //Hatch
      error_cnt++;
      if(error_cnt > hatch_max_time){
        Motor_OFF(7);
        Motor_OFF(8);
        
        state_data1 = 0x00;
        state_data2 = 0x00;
        state_data3 = 0x00;
        cmd = (byte)0x52;
      }
      break;
    case (byte)0x04:  //Around Bar
      error_cnt++;
      if(error_cnt > around_bar_max_time){
        Motor_OFF(1);
        Motor_OFF(2);
        Motor_OFF(3);
        Motor_OFF(4);
        
        state_data1 = 0x00;
        state_data2 = 0x00;
        state_data3 = 0x00;
        cmd = (byte)0x52;
      }
      break;
    case (byte)0x02:  //Lift
      error_cnt++;
      if(error_cnt > lift_max_time){
        Motor_OFF(6);
        
        state_data1 = 0x00;
        state_data2 = 0x00;
        state_data3 = 0x00;
        cmd = (byte)0x52;
      }
      break;
    case (byte)0x01:  //Tilting
      error_cnt++;
      if(error_cnt > hatch_max_time){
        Motor_OFF(5);
        
        state_data1 = 0x00;
        state_data2 = 0x00;
        state_data3 = 0x00;
        cmd = (byte)0x52;
      }
      break;
  }
  addr_past = addr;
}

void setup() {
  // D6 : Tilting, D8 : Motor7, D9 : Motor8의 Hz 변화 시키는 코드
  //TCCR0B = TCCR0B & B11111000 | B00000010;
  // D4, D13 for PWM frequency of 7812.50 Hz
  //TCCR1B = TCCR1B & B11111000 | B00000010;
  // D11, D12 for PWM frequency of 3921.16 Hz
  
  TCCR2B = TCCR2B & B11111000 | B00000010;
  // D9, D10 for  PWM frequency of 3921.16 Hz
  
  //TCCR3B = TCCR3B & B11111000 | B00000010;
  // D2, D3, D5 for PWM frequency of 3921.16 Hz, 코드 적용 시 Motor(5)(= D6)가 제대로 동작하지 않음.
  
  TCCR4B = TCCR4B & B11111000 | B00000010;
  // D6, D7, D8 for PWM frequency of 3921.16 Hz
  
  // 모터
  pinMode(VEL1, OUTPUT);
  pinMode(VEL2, OUTPUT);
  pinMode(VEL3, OUTPUT);
  pinMode(VEL4, OUTPUT);
  pinMode(VEL5, OUTPUT);
  pinMode(VEL6, OUTPUT);
  pinMode(VEL7, OUTPUT);
  pinMode(VEL8, OUTPUT);
  pinMode(RD1, OUTPUT);
  pinMode(LS1, OUTPUT);
  pinMode(RD2, OUTPUT);
  pinMode(LS2, OUTPUT);
  pinMode(RD3, OUTPUT);
  pinMode(LS3, OUTPUT);
  pinMode(RD4, OUTPUT);
  pinMode(LS4, OUTPUT);
  pinMode(RD5, OUTPUT);
  pinMode(LS5, OUTPUT);
  pinMode(RD6, OUTPUT);
  pinMode(LS6, OUTPUT);
  pinMode(RD7, OUTPUT);
  pinMode(LS7, OUTPUT);
  pinMode(RD8, OUTPUT);
  pinMode(LS8, OUTPUT);
  // 포토센서 (디지털)
  pinMode(Photo_0, INPUT);
  pinMode(Photo_1, INPUT);
  pinMode(Photo_2, INPUT);
  pinMode(Photo_3, INPUT);
  pinMode(Photo_4, INPUT);
  pinMode(Photo_5, INPUT);
  pinMode(Photo_6, INPUT);
  pinMode(Photo_7, INPUT);
  pinMode(Photo_8, INPUT);
  pinMode(Photo_9, INPUT);
  pinMode(Photo_10, INPUT);
  pinMode(Photo_11, INPUT);
  pinMode(Photo_12, INPUT);
  pinMode(Photo_13, INPUT);
  pinMode(Photo_14, INPUT);
  pinMode(Photo_15, INPUT);
  pinMode(Photo_16, INPUT);
  pinMode(Photo_17, INPUT);
  pinMode(Photo_18, INPUT);
  pinMode(Photo_19, INPUT);
  //초기 셋팅
  digitalWrite(VEL1, LOW);
  digitalWrite(VEL2, LOW);
  digitalWrite(VEL3, LOW);
  digitalWrite(VEL4, LOW);
  digitalWrite(VEL5, LOW);
  digitalWrite(VEL6, LOW);
  digitalWrite(VEL7, LOW);
  digitalWrite(VEL8, LOW);
  digitalWrite(RD1, HIGH);
  digitalWrite(LS1, HIGH);
  digitalWrite(RD2, HIGH);
  digitalWrite(LS2, HIGH);
  digitalWrite(RD3, HIGH);
  digitalWrite(LS3, HIGH);
  digitalWrite(RD4, HIGH);
  digitalWrite(LS4, HIGH);
  digitalWrite(RD5, HIGH);
  digitalWrite(LS5, HIGH);
  digitalWrite(RD6, HIGH);
  digitalWrite(LS6, HIGH);
  digitalWrite(RD7, HIGH);
  digitalWrite(LS7, HIGH);
  digitalWrite(RD8, HIGH);
  digitalWrite(LS8, HIGH);
  
  Serial.begin(9600);

  //1[sec]마다 Error_check 함수를 실행하여, 하드웨어 고장 또는 오류나면 모터 동작 멈추게 함
  MsTimer2::set(1000, Error_check);
  MsTimer2::start();

  State_read();
}

void loop() {  
  if(SerFlag==0){
    RecievePacket();
  }    
  else{
    PacketAnalyze();
    switch(addr){
      case (byte)0x08:  //Hatch
        //Hatch Open
        if(data1 == (byte)0x08){
          Motor_Slow_CCW(7);
          Motor_Slow_CCW(8);
          /*
          while((photo_flag[3]<3)||(photo_flag[13]<3)){
            if((photo_flag[3]<3)&&(Hatch_flag[0]==0)){
              photo_flag[3] = Check_Digital_0(Photo_3);
            }
            if((photo_flag[3]==3)&&(Hatch_flag[0]==0)){          
              Motor_OFF(8);
              photo_cnt[0] = 0;
              Hatch_flag[0] = 1;
            }
            if((photo_flag[13]<3)&&(Hatch_flag[1]==0)){
              photo_flag[13] = Check_Digital_1(Photo_13);
            }
            if((photo_flag[13]==3)&&(Hatch_flag[1]==0)){
              Motor_OFF(7);
              photo_cnt[1] = 0;
              Hatch_flag[1] = 1;
            }
            //timeout
            if(error_cnt > hatch_max_time){
              break;
            }
            delay(10);            
          }
          */
          
          // flag 값 초기화          
          for(int i=0; i<20; i++){
            photo_flag[i] = 0;
            if(i<2)
              Hatch_flag[i] = 0;
          }
          if(error_cnt <= hatch_max_time){
            state_data1 = 0x11;
          }
        }
        //Hatch Close
        else if(data1 == (byte)0x04){
          Motor_Slow_CW(7);
          Motor_Slow_CW(8);
          /*
          while((photo_flag[3]<3)||(photo_flag[13]<3)){        
            if((photo_flag[3]<3)&&(Hatch_flag[0]==0)){
              photo_flag[3] = Check_Digital_0(Photo_3);        
            }
            if((photo_flag[3]==3)&&(Hatch_flag[0]==0)){
              Motor_OFF(8);
              photo_cnt[0] = 0;
              Hatch_flag[0] = 1;
            }        
            if((photo_flag[13]<3)&&(Hatch_flag[1]==0)){
              photo_flag[13] = Check_Digital_1(Photo_13);
            }
            if((photo_flag[13]==3)&&(Hatch_flag[1]==0)){
              Motor_OFF(7);
              photo_cnt[1] = 0;
              Hatch_flag[1] = 1;
            }
            if(error_cnt > hatch_max_time){
              break;
            }
            delay(10);
          }
          */
          
          for(int i=0; i<20; i++){
            photo_flag[i] = 0;
            if(i<2)
              Hatch_flag[i] = 0;
          }
          if(error_cnt <= hatch_max_time){
            state_data1 = 0x10;
          }
        }
        //cmd 값 변경
        if(error_cnt <= hatch_max_time){
          cmd = (byte)0x51;
        }
        
        break;
        
      case (byte)0x04:  //Around Bar
        //Around Bar Fasten
        if(data1 == (byte)0x08){
          Motor_Slow2_CW(1);
          Motor_Slow2_CW(3);
          /*
          while((Arrange_flag[0]==0)||(Arrange_flag[1]==0)){
            if((photo_flag[2]==0)&&(Arrange_flag[0]==0)){
              photo_flag[2] = Check_Digital_0(Photo_2);
            }
            if((photo_flag[2]==1)&&(Arrange_flag[0]==0)){
              Motor_OFF(1);
              photo_cnt[0] = 0;
              Arrange_flag[0] = 1;
            }        
            if((photo_flag[4]==0)&&(Arrange_flag[1]==0)){
              photo_flag[4] = Check_Digital_1(Photo_4);
            }
            if((photo_flag[4]==1)&&(Arrange_flag[1]==0)){
              Motor_OFF(3);
              photo_cnt[1] = 0;
              Arrange_flag[1] = 1;
            }
            if(error_cnt > around_bar_max_time){
              break;
            }
          }
          */
                
          Motor_OFF(1);
          Motor_OFF(3);
          delay(300);

          Motor_Slow2_CW(2);
          Motor_Slow2_CW(4);
          /*
          while((Arrange_flag[2]==0)||(Arrange_flag[3]==0)){        
            if((photo_flag[9]==0)&&(Arrange_flag[2]==0)){
              photo_flag[9] = Check_Digital_2(Photo_9);
            }
            if((photo_flag[9]==1)&&(Arrange_flag[2]==0)){
              Motor_OFF(2);
              photo_cnt[2] = 0;
              Arrange_flag[2] = 1;
            }        
            if((photo_flag[10]==0)&&(Arrange_flag[3]==0)){
              photo_flag[10] = Check_Digital_3(Photo_10);
            }
            if((photo_flag[10]==1)&&(Arrange_flag[3]==0)){
              Motor_OFF(4);
              photo_cnt[3] = 0;
              Arrange_flag[3] = 1;
            }
            if(error_cnt > around_bar_max_time){
              break;
            }
          }
          */
          
          Motor_OFF(2);
          Motor_OFF(4);
      
          for(int i=0; i<20; i++){
            photo_flag[i] = 0;
            if(i<4){
              Arrange_flag[i] = 0;
              photo_cnt[i] = 0;
            }
          }
          if(error_cnt <= around_bar_max_time){
            state_data2 = 0x11;
          }
        }
        //Around Bar Relax
        else if(data1 == (byte)0x04){
          Motor_Slow2_CCW(1);
          Motor_Slow2_CCW(3);
          /*
          while((Arrange_flag[0]==0)||(Arrange_flag[1]==0)){
            if((photo_flag[1]==0)&&(Arrange_flag[0]==0)){
              photo_flag[1] = Check_Digital_0(Photo_1);
            }
            if((photo_flag[1]==1)&&(Arrange_flag[0]==0)){
              Motor_OFF(1);
              Arrange_flag[0] = 1;
            }       
            if((photo_flag[5]==0)||(Arrange_flag[1]==0)){
              photo_flag[5] = Check_Digital_1(Photo_5);
            }
            if((photo_flag[5]==1)&&(Arrange_flag[1]==0)){
              Motor_OFF(3);
              Arrange_flag[1] = 1;
            }
            if(error_cnt > around_bar_max_time){
              break;
            }
          }
          */
          
          Motor_OFF(1);
          Motor_OFF(3);
          delay(300);
      
          Motor_Slow2_CCW(2);
          Motor_Slow2_CCW(4);
          
          /*
          while((Arrange_flag[2]==0)||(Arrange_flag[3]==0)){
            if((photo_flag[8]==0)&&(Arrange_flag[2]==0)){
              photo_flag[8] = Check_Digital_2(Photo_8);
            }
            if((photo_flag[8]==1)&&(Arrange_flag[2]==0)){
              Motor_OFF(2);
              Arrange_flag[2] = 1;
            }        
            if((photo_flag[11]==0)||(Arrange_flag[3]==0)){
              photo_flag[11] = Check_Digital_3(Photo_11);
            }
            if(photo_flag[11]==1&&(Arrange_flag[3]==0)){
              Motor_OFF(4);
              Arrange_flag[3] = 1;
            }
            if(error_cnt > around_bar_max_time){
              break;
            }
          }
          */
          
          Motor_OFF(2);
          Motor_OFF(4);
         
          for(int i=0; i<20; i++){
            photo_flag[i] = 0;
            if(i<4){
              Arrange_flag[i] = 0;
              photo_cnt[i] = 0;
            }
          }
          if(error_cnt <= around_bar_max_time){
            state_data2 = 0x10;
          }
        }
        if(error_cnt <= around_bar_max_time){
          cmd = (byte)0x51;
        }
        break;
        
      case (byte)0x02:  //Lift
        //Lift 1st floor
        if(data1 == (byte)0x08){
          Motor_CCW(6);
          /*
          while(photo_flag[17]==0){
            photo_flag[17] = Check_Digital_0(Photo_17);
            if(error_cnt > lift_max_time){
              break;
            }
          }
          */
          
          delay(600);
          Motor_OFF(6);
          
          photo_cnt[0] = 0;
          for(int i=0; i<20; i++){
            photo_flag[i] = 0;
          }
          if(error_cnt <= lift_max_time){
            state_data3 = 0x11;
          }
        }
        //Lift 2nd floor
        else if(data1 == (byte)0x04){
          Motor_CCW(6);
          /*
          while(photo_flag[16]==0){
            photo_flag[16] = Check_Digital_0(Photo_16);
            if(error_cnt > lift_max_time){
              break;
            }
          }
          */
          Motor_OFF(6);
                
          photo_cnt[0] = 0;
          for(int i=0; i<20; i++){
            photo_flag[i] = 0;
          }
          if(error_cnt <= lift_max_time){
            state_data3 = 0x12;
          }
        }
        //Lift Return
        else if(data1 == (byte)0x02){
          Motor_CW(6);
          /*
          while(photo_flag[14]==0){
            photo_flag[14] = Check_Digital_0(Photo_14);
            if(error_cnt > lift_max_time){
              break;
            }
          }
          */
          Motor_OFF(6);
          
          photo_cnt[0] = 0;
          for(int i=0; i<20; i++){
            photo_flag[i] = 0;
          }
          if(error_cnt <= lift_max_time){
            state_data3 = 0x10;
          }
        }
        if(error_cnt <= lift_max_time){
          cmd = (byte)0x51;
        }
        break;
        
      case (byte)0x01:  //Tilting
        //Tilting Right
        if(data1 == (byte)0x08){
          Motor_CCW(5);
          delay(250);
          Motor_OFF(5);
          delay(7000);
          Motor_CW(5);
          delay(250);
          Motor_OFF(5);
          
          for(int i=0; i<20; i++){
            photo_flag[i] = 0;
          }
        }
        //Tilting Left
        else if(data1 == (byte)0x04){
          Motor_CW(5);
          delay(250);
          Motor_OFF(5);
          delay(7000);
          Motor_CCW(5);
          delay(250);
          Motor_OFF(5);
          for(int i=0; i<20; i++){
            photo_flag[i] = 0;
          }
        }
        if(error_cnt <= tilting_max_time){
          cmd = (byte)0x51;
        }
        break;
        
      case (byte)0x10:  //Motor
        //Motor1 CW
        if(data1 == (byte)0x10){
          Motor_CW(1);
          delay((int)data2*1000);
          Motor_OFF(1);
        }
        //Motor1 CCW
        else if(data1 == (byte)0x11){
          Motor_CCW(1);
          delay((int)data2*1000);
          Motor_OFF(1);
        }
        //Motor2 CW
        else if(data1 == (byte)0x12){
          Motor_CW(2);
          delay((int)data2*1000);
          Motor_OFF(2);
        }
        //Motor2 CCW
        else if(data1 == (byte)0x13){
          Motor_CCW(2);
          delay((int)data2*1000);
          Motor_OFF(2);
        }
        //Motor3 CW
        else if(data1 == (byte)0x14){
          Motor_CW(3);
          delay((int)data2*1000);
          Motor_OFF(3);
        }
        //Motor3 CCW
        else if(data1 == (byte)0x15){
          Motor_CCW(3);
          delay((int)data2*1000);
          Motor_OFF(3);
        }
        //Motor4 CW
        else if(data1 == (byte)0x16){
          Motor_CW(4);
          delay((int)data2*1000);
          Motor_OFF(4);
        }
        //Motor4 CCW
        else if(data1 == (byte)0x17){
          Motor_CCW(4);
          delay((int)data2*1000);
          Motor_OFF(4);
        }
        //Motor5 CW
        else if(data1 == (byte)0x18){
          Motor_CW(5);
          delay((int)data2*1000);
          Motor_OFF(5);
        }
        //Motor5 CCW
        else if(data1 == (byte)0x19){
          Motor_CCW(5);
          delay((int)data2*1000);
          Motor_OFF(5);
        }
        //Motor6 CW
        else if(data1 == (byte)0x1A){
          Motor_CW(6);
          delay((int)data2*1000);
          Motor_OFF(6);
        }
        //Motor6 CCW
        else if(data1 == (byte)0x1B){
          Motor_CCW(6);
          delay((int)data2*1000);
          Motor_OFF(6);
        }
        //Motor7 CW
        else if(data1 == (byte)0x1C){
          Motor_Slow_CW(7);
          delay((int)data2*1000);
          Motor_OFF(7);
        }
        //Motor7 CCW
        else if(data1 == (byte)0x1D){
          Motor_Slow_CCW(7);
          delay((int)data2*1000);
          Motor_OFF(7);
        }
        //Motor8 CW
        else if(data1 == (byte)0x1E){
          Motor_Slow_CW(8);
          delay((int)data2*1000);
          Motor_OFF(8);
        }
        //Motor8 CCW
        else if(data1 == (byte)0x1F){
          Motor_Slow_CCW(8);
          delay((int)data2*1000);
          Motor_OFF(8);
        }
        cmd = (byte)0x51;
        break;
        
      case (byte)0x20:  //State
        //State Check
        if(cmd == (byte)0x60 && data1 == (byte)0x10){
          cmd = (byte)0x61;
        }
        //State Reset
        else if(cmd == (byte)0x60 && data1 == (byte)0x11){
          state_data1 = 0x10;
          state_data2 = 0x10;
          state_data3 = 0x10;
          cmd = (byte)0x61;
        }
        break;
    }
    State_save();
    SenPacket();
    Reset_protocol();
  }
}
