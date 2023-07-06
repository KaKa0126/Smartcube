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
int temp_val[20]={0,}, val[20]={0,}, photo_flag[20]={0,}, photo_cnt[4]={0,}, num_c=0, photo=0;
int detect_flag[20]={0,}, num[20]={0,}, arr[20][2]={0,}; 
int Hatch_flag[2]={0,}, Arrange_flag[4]={0,};

char input = ' ', ch = ' ', temp_ch=' ';
    
// 모터
void Motor_CW(int num){
  digitalWrite((num+1), HIGH);
  digitalWrite((20+num*2), LOW);
  digitalWrite((21+num*2), LOW);
  Serial.print("//---------------------Motor_CW ");
  Serial.println(num);
}

void Motor_CCW(int num){
  digitalWrite((num+1), HIGH);
  digitalWrite((20+num*2), HIGH);
  digitalWrite((21+num*2), LOW);
  Serial.print("//---------------------Motor_CCW ");
  Serial.println(num);
}

void Motor_OFF(int num){
  digitalWrite((num+1), LOW);
  digitalWrite((20+num*2), HIGH);
  digitalWrite((21+num*2), HIGH);

  
  Serial.print("//---------------------Motor_STOP ");
  Serial.println(num);
}

void Motor_Slow_CW(int num){
  analogWrite((num+1), 100);
  digitalWrite((20+num*2), LOW);
  digitalWrite((21+num*2), LOW);
  Serial.print("//---------------------Motor_Slow_CW ");
  Serial.println(num);
}

void Motor_Slow_CCW(int num){
  analogWrite((num+1), 100);
  digitalWrite((20+num*2), HIGH);
  digitalWrite((21+num*2), LOW);
  Serial.print("//---------------------Motor_Slow_CCW ");
  Serial.println(num);
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
    Serial.print("--//Detect        ||   ");
    Serial.println(photo_cnt[0]);
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
    Serial.print("--//Detect        || ");
    Serial.println(photo_cnt[1]);
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
    Serial.print("--//Detect        || ");
    Serial.println(photo_cnt[2]);
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
    Serial.print("--//Detect        || ");
    Serial.println(photo_cnt[3]);
  }
  temp_val[num] = val[num];

  return photo_cnt[3];
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
  // D2, D3, D5 for PWM frequency of 3921.16 Hz
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
}

void loop() {
  // put your main code here, to run repeatedly:

  input = Serial.read();

  switch(input){
    case 'a':     //Hatch_Open
      Serial.println("Hatch_Open");
      Serial.println("----------");
      Motor_Slow_CCW(7);
      Motor_Slow_CCW(8);
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

        delay(10);
      }
      for(int i=0; i<20; i++){
        photo_flag[i] = 0;
        if(i<2)
          Hatch_flag[i] = 0;
      }   // flag 값 초기화
      break;

    case 's':     //Hatch_Close
      Serial.println("Hatch_Close");
      Serial.println("----------");
      Motor_Slow_CW(7);
      Motor_Slow_CW(8);
      while((photo_flag[3]<3)||(photo_flag[13]<3)){
        
        if((photo_flag[3]<3)&&(Hatch_flag[0]==0)){
          photo_flag[3] = Check_Digital_0(Photo_3);        
        }
        if((photo_flag[3]==3)&&(Hatch_flag[0]==0)){
          //delay(100);
          Motor_OFF(8);
          photo_cnt[0] = 0;
          Hatch_flag[0] = 1;
        }
        
        if((photo_flag[13]<3)&&(Hatch_flag[1]==0)){
          photo_flag[13] = Check_Digital_1(Photo_13);
        }
        if((photo_flag[13]==3)&&(Hatch_flag[1]==0)){
          //delay(150);
          Motor_OFF(7);
          photo_cnt[1] = 0;
          Hatch_flag[1] = 1;
        }
        
        delay(10);
      }
      for(int i=0; i<20; i++){
        photo_flag[i] = 0;
        if(i<2)
          Hatch_flag[i] = 0;
      }
      break;

    case 'd':   //정렬
      Serial.println("Fasten_Bar");
      Serial.println("----------");
      Motor_CW(1);
      Motor_CW(3);
      
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
      }
      Motor_OFF(1);
      Motor_OFF(3);
      delay(300);

      Motor_CW(2);
      Motor_CW(4);
      
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
      }
      Motor_OFF(2);
      Motor_OFF(4);
      
      for(int i=0; i<20; i++){
        photo_flag[i] = 0;
        if(i<4){
          Arrange_flag[i] = 0;
          photo_cnt[i] = 0;
        }
      }
      break;

    case 'f':   //relex 정렬
      Serial.println("Relex_Bar");
      Serial.println("----------");
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
        
        
        //delay(10);
      }
      Motor_OFF(2);
      Motor_OFF(4);
      for(int i=0; i<20; i++){
        photo_flag[i] = 0;
        if(i<4){
          Arrange_flag[i] = 0;
          photo_cnt[i] = 0;
        }
      }
      break;

    case 'g':   //Lift_1
      Serial.println("Lift_1st");
      Serial.println("----------");
      Motor_CCW(6);
      while(photo_flag[17]==0){
        photo_flag[17] = Check_Digital_0(Photo_17);
        //delay(10);
      }
      delay(600);
      Motor_OFF(6);
      photo_cnt[0] = 0;

      for(int i=0; i<20; i++)
        photo_flag[i] = 0;
      break;

    case 'h':   //Lift_2
      Serial.println("Lift_2nd");
      Serial.println("----------");
      Motor_CCW(6);
      while(photo_flag[16]==0){
        if(photo_flag[16]==0){
          photo_flag[16] = Check_Digital_0(Photo_16);
          //delay(10);
        }  
        
      }
      Motor_OFF(6);
      
      photo_cnt[0] = 0;

      for(int i=0; i<20; i++)
        photo_flag[i] = 0;
      break;

    case 'j':   //Lift_Return
      Serial.println("Lift_Return");
      Serial.println("----------");
      Motor_CW(6);
      while(photo_flag[14]==0){
        photo_flag[14] = Check_Digital_0(Photo_14);
        //delay(10);
      }
      Motor_OFF(6);
      photo_cnt[0] = 0;

      for(int i=0; i<20; i++)
        photo_flag[i] = 0;
      break;

    case 'k':   //Tilting_R
      Serial.println("Tilting_R");
      Serial.println("----------");
      Motor_CCW(5);
      /*
      while(photo_flag[19]<2){
        photo_flag[19] = Check_Digital_0(Photo_19);
        
        
        delay(10);
      }
      Motor_OFF(5);
      for(int i=0; i<20; i++){
        photo_flag[i] = 0;
      }
      delay(3000);
      
      
      Motor_CW(5);
      Serial.println(photo_flag[19]);
      while(photo_flag[19]<2){
        photo_flag[19] = Check_Digital_0(Photo_19);
        delay(10);
      }
      Motor_OFF(5);*/
      delay(250);
      Motor_OFF(5);
      delay(5000);
      Motor_CW(5);
      delay(250);
      Motor_OFF(5);
      for(int i=0; i<20; i++)
        photo_flag[i] = 0;
      break;

    case 'l':   //Tilting_L
      Serial.println("Tilting_L");
      Serial.println("----------");
      Motor_CW(5);
      /*
      while(photo_flag[19]<2){
        photo_flag[19] = Check_Digital_0(Photo_19);
        delay(10);
      }
      Motor_OFF(5);
      for(int i=0; i<20; i++){
        photo_flag[i] = 0;
      }
      delay(3000);

      Motor_CCW(5);
      Serial.println(photo_flag[19]);
      while(photo_flag[19]<2){
        photo_flag[19] = Check_Digital_0(Photo_19);
        delay(10);
      }
      Motor_OFF(5);
      */
      delay(250);
      Motor_OFF(5);
      delay(5000);
      Motor_CCW(5);
      delay(250);
      Motor_OFF(5);
      for(int i=0; i<20; i++)
        photo_flag[i] = 0;
      break;

    case '0':   //All STOP
      Motor_OFF(1);
      Motor_OFF(2);
      Motor_OFF(3);
      Motor_OFF(4);
      Motor_OFF(5);
      Motor_OFF(6);
      Motor_OFF(7);
      Motor_OFF(8);
      break;

    case '1':
      Motor_CW(1);
      break;

    case '2':
      Motor_CW(2);
      break;

    case '3':
      Motor_CW(3);
      break;

    case '4':
      Motor_CW(4);
      break;

    case '5':
      Motor_CW(5);
      break;

    case '6':
      Motor_CW(6);
      break;

    case '7':
      Motor_CW(7);
      break;
      
    case '8':
      Motor_CW(8);
      break;

    case 'q':
      Motor_CCW(1);
      break;

    case 'w':
      Motor_CCW(2);
      break;

    case 'e':
      Motor_CCW(3);
      break;

    case 'r':
      Motor_CCW(4);
      break;

    case 't':
      Motor_CCW(5);
      break;

    case 'y':
      Motor_CCW(6);
      break;

    case 'u':
      Motor_CCW(7);
      break;
      
    case 'i':
      Motor_CCW(8);
      break;      
  }
  input = ' ';
}
