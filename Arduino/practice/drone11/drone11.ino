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

//RecPacket[] : 수신된 프로토콜 배열
//SendPacket[] : 송부할 프로토콜 배열
byte RecPacket[9]={0, }, SendPacket[9]={0, };

//key : 수신된 프로토콜 배열 내의 갯수
//SerFlag : 시리얼통신 중이면 1, 아니면 0
int key=0, SerFlag=0;

//프로토콜 변수
byte stx, addr, cmd, data1, data2, data3, data4, etx, sum;

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
      (RecPacket[1]==0x08||RecPacket[1]==0x04||RecPacket[1]==0x02||RecPacket[1]==0x01)&&
      RecPacket[2]==0x50&&
      (RecPacket[3]==0x08||RecPacket[3]==0x04||RecPacket[3]==0x02)&&
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
  SendPacket[2] = 0x51;
  SendPacket[3] = data1;
  SendPacket[4] = data2;
  SendPacket[5] = data3;
  SendPacket[6] = data4;
  SendPacket[7] = etx; 
  SendPacket[8] = stx+addr+SendPacket[2]+data1+data2+data3+data4+etx;

  if(stx != 0){
    Serial.write(SendPacket, 9);
  }
}

void setup() {
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

  Motor_CCW(1);
  Motor_CCW(3);
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
  }
  Motor_OFF(1);
  Motor_OFF(3);
  delay(300);
      
  Motor_CCW(2);
  Motor_CCW(4);
      
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
  }
  Motor_OFF(2);
  Motor_OFF(4);
}

void loop() {

}
