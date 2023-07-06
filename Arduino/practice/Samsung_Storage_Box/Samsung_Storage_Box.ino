/*
  Project Name : Samsung_Storage_Box
  Reporting date : 23.03.06
  Update date : 23.03.28
  written by Smartcube HW Staff 'HEONSEOK HA'
*/

/*
  - update details -
  2023.03.06 ~ 2023.03.09 : Drafting
  2023.03.23 ~ 2023.03.28 : Test and Modify

  - HW -
  1. ATmega2560
  2. KDC248H, motor control board
  3. NER-11343, Relay-M1(CH8)-5V(8CH Relay Module)
 */

#define VEL 2
#define RD 3
#define LR 4
#define RELAY1 22
#define RELAY2 23
#define RELAY3 24
#define RELAY4 25
#define RELAY5 26
#define RELAY6 27

//함 모터 동시 동작 시, 제어 변수 설정
int cnt0 = 0, cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0, cnt5 = 0, cnt6 = 0, cnt7 = 0, cnt8 = 0, cnt9 = 0, cnt10 = 0, cnt11 = 0;
int try0 = 0, try1 = 0, try2 = 0, try3 = 0, try4 = 0, try5 = 0, try6 = 0, try7 = 0, try8 = 0, try9 = 0, try10 = 0, try11 = 0;

/*
 * 근접센서 감지
 * A0~A11 : 1열 당, 12개 사용(추후, 1열 당 14개 사용 예정)
 * 
 * 동작하지 않은 초기 상태 : Start Sensor가 인지되어 정지된 상태
 * 동작하여 움직인 상태 : End Sensor가 인지되어 정지된 상태
 * 
 * A0 : 1함 Start Sensor
 * A1 : 1함 End Sensor
 * A2 : 2함 Start Sensor
 * A3 : 2함 End Sensor
 * A4 : 3함 Start Sensor
 * A5 : 3함 End Sensor
 * A6 : 4함 Start Sensor
 * A7 : 4함 End Sensor
 * A8 : 5함 Start Sensor
 * A9 : 5함 End Sensor
 * A10 : 열 End Sensor
 * A11 : 열 Start Sensor
 * 
 */

// 함과 열을 움직이는 모터에 인가되는 전원 설정 파라미터
//열 모터의 Voltage는 24V로 고정(HIGH)
int door_voltage_rate1 = 75;  //1함
int door_voltage_rate3 = 45;  //3함
int door_voltage_rate4 = 55;  //4함
int door_voltage_rate5 = 40;  //2함,5함

// 모터
void Door_Motor_CW1(){
  analogWrite(VEL, door_voltage_rate1);
  digitalWrite(RD, LOW);
  digitalWrite(LR, LOW);
}

void Door_Motor_CCW1(){
  analogWrite(VEL, door_voltage_rate1);
  digitalWrite(RD, HIGH);
  digitalWrite(LR, LOW);
}

void Door_Motor_CW3(){
  analogWrite(VEL, door_voltage_rate3);
  digitalWrite(RD, LOW);
  digitalWrite(LR, LOW);
}

void Door_Motor_CCW3(){
  analogWrite(VEL, door_voltage_rate3);
  digitalWrite(RD, HIGH);
  digitalWrite(LR, LOW);
}

void Door_Motor_CW4(){
  analogWrite(VEL, door_voltage_rate4);
  digitalWrite(RD, LOW);
  digitalWrite(LR, LOW);
}

void Door_Motor_CCW4(){
  analogWrite(VEL, door_voltage_rate4);
  digitalWrite(RD, HIGH);
  digitalWrite(LR, LOW);
}

void Door_Motor_CW5(){
  analogWrite(VEL, door_voltage_rate5);
  digitalWrite(RD, LOW);
  digitalWrite(LR, LOW);
}

void Door_Motor_CCW5(){
  analogWrite(VEL, door_voltage_rate5);
  digitalWrite(RD, HIGH);
  digitalWrite(LR, LOW);
}

void Column_Motor_CW(){
  digitalWrite(VEL, HIGH);
  digitalWrite(RD, LOW);
  digitalWrite(LR, LOW);
}

void Column_Motor_CCW(){
  digitalWrite(VEL, HIGH);
  digitalWrite(RD, HIGH);
  digitalWrite(LR, LOW);
}

void Motor_OFF(){
  digitalWrite(VEL, LOW);
  digitalWrite(RD, HIGH);
  digitalWrite(LR, HIGH);
}

void brocker(char req)
{
  /*
   * Q : 함 모터(1) 열기
   * W : 함 모터(2) 열기
   * E : 함 모터(3) 열기
   * R : 함 모터(4) 열기
   * T : 함 모터(5) 열기
   * Y : 열 모터(6) 열기
   * P : 전체 함 모터(1~5) 열기
   * 
   * A : 함 모터(1) 닫기
   * S : 함 모터(2) 닫기
   * D : 함 모터(3) 닫기
   * F : 함 모터(4) 닫기
   * G : 함 모터(5) 닫기
   * H : 열 모터(6) 닫기
   * L : 전체 함 모터(1~5) 닫기
   */
  if (req == 'Q')
  {
    digitalWrite(RELAY1, HIGH);
    delay(1000);
    
    Door_Motor_CW1();
    delay(10000);

    while(analogRead(A1) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000); (추후, 넣었을 때, 안넣었을 때, 움직임 비교 필요)
    digitalWrite(RELAY1, LOW);
  }
  else if (req == 'W')
  {
    digitalWrite(RELAY2, HIGH);
    delay(1000);
    
    Door_Motor_CW5();
    delay(5000);
    
    while(analogRead(A3) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY2, LOW);
  }
  else if (req == 'E')
  {
    digitalWrite(RELAY3, HIGH);
    delay(1000);
    
    Door_Motor_CW3();
    delay(5000);

    while(analogRead(A5) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY3, LOW);
  }
  else if (req == 'R')
  {
    digitalWrite(RELAY4, HIGH);
    delay(1000);
    
    Door_Motor_CW4();
    delay(5000);

    while(analogRead(A7) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY4, LOW);
  }
  else if (req == 'T')
  {
    digitalWrite(RELAY5, HIGH);
    delay(1000);
    
    Door_Motor_CW5(); 
    delay(5000);

    while(analogRead(A9) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY5, LOW);
  }
  else if (req == 'Y')
  {
    digitalWrite(RELAY6, HIGH);
    delay(1000);
    
    Column_Motor_CW();
    delay(5000);

    while(analogRead(A10) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY6, LOW);
  }
  else if (req == 'P')
  {
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY3, HIGH);
    digitalWrite(RELAY4, HIGH);
    digitalWrite(RELAY5, HIGH);
    delay(1000);
    
    Door_Motor_CW1();
    delay(5000);

    while((cnt1 != 1)||(cnt3 != 1)||(cnt5 != 1)||(cnt7 != 1)||(cnt9 != 1)){
      if(analogRead(A1) == 0 && try1 == 0){
        digitalWrite(RELAY1, LOW);
        cnt1 = 1;
        try1 = 1;
      }
      if(analogRead(A3) == 0 && try3 == 0){
        digitalWrite(RELAY2, LOW);
        cnt3 = 1;
        try3 = 1;
      }
      if(analogRead(A5) == 0 && try5 == 0){
        digitalWrite(RELAY3, LOW);
        cnt5 = 1;
        try5 = 1;
      }
      if(analogRead(A7) == 0 && try7 == 0){
        digitalWrite(RELAY4, LOW);
        cnt7 = 1;
        try7 = 1;
      }
      if(analogRead(A9) == 0 && try9 == 0){
        digitalWrite(RELAY5, LOW);
        cnt9 = 1;
        try9 = 1;
      }
      delay(10);
    }

    //제어 변수 초기화
    cnt0 = 0, cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0, cnt5 = 0, cnt6 = 0, cnt7 = 0, cnt8 = 0, cnt9 = 0, cnt10 = 0, cnt11 = 0;
    try0 = 0, try1 = 0, try2 = 0, try3 = 0, try4 = 0, try5 = 0, try6 = 0, try7 = 0, try8 = 0, try9 = 0, try10 = 0, try11 = 0;
    
    Motor_OFF();
  }
  else if (req == 'A')
  {    
    digitalWrite(RELAY1, HIGH);
    delay(1000);
    
    Door_Motor_CCW1();
    delay(10000);

    while(analogRead(A0) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY1, LOW);
  }
  else if (req == 'S')
  {
    digitalWrite(RELAY2, HIGH);
    delay(1000);
    
    Door_Motor_CCW5();
    delay(5000);

    while(analogRead(A2) != 0){
      delay(10);
    }

    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY2, LOW);
  }
  else if (req == 'D')
  {
    digitalWrite(RELAY3, HIGH);
    delay(1000);
    
    Door_Motor_CCW3();
    delay(5000);

    while(analogRead(A4) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY3, LOW);
  }
  else if (req == 'F')
  {
    digitalWrite(RELAY4, HIGH);
    delay(1000);
    
    Door_Motor_CCW4();
    delay(5000);
    
    while(analogRead(A6) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY4, LOW);
  }
  else if (req == 'G')
  {
    digitalWrite(RELAY5, HIGH);
    delay(1000);
    
    Door_Motor_CCW5();
    delay(5000);

    while(analogRead(A8) != 0){
      delay(10);
    }

    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY5, LOW);
  }
  else if (req == 'H')
  {
    digitalWrite(RELAY6, HIGH);
    delay(1000);
    
    Column_Motor_CCW();
    delay(5000);

    while(analogRead(A11) != 0){
      delay(10);
    }
    
    Motor_OFF();
    //delay(1000);
    digitalWrite(RELAY6, LOW);
  }
  else if (req == 'L')
  {
    digitalWrite(RELAY1, HIGH);
    digitalWrite(RELAY2, HIGH);
    digitalWrite(RELAY3, HIGH);
    digitalWrite(RELAY4, HIGH);
    digitalWrite(RELAY5, HIGH);
    delay(1000);
    
    Door_Motor_CCW1();
    delay(5000);

    while((cnt0 != 1)||(cnt2 != 1)||(cnt4 != 1)||(cnt6 != 1)||(cnt8 != 1)){
      if(analogRead(A0) == 0 && try0 == 0){
        digitalWrite(RELAY1, LOW);
        cnt0 = 1;
        try0 = 1;
      }
      if(analogRead(A2) == 0 && try2 == 0){
        digitalWrite(RELAY2, LOW);
        cnt2 = 1;
        try2 = 1;
      }
      if(analogRead(A4) == 0 && try4 == 0){
        digitalWrite(RELAY3, LOW);
        cnt4 = 1;
        try4 = 1;
      }
      if(analogRead(A6) == 0 && try6 == 0){
        digitalWrite(RELAY4, LOW);
        cnt6 = 1;
        try6 = 1;
      }
      if(analogRead(A8) == 0 && try8 == 0){
        digitalWrite(RELAY5, LOW);
        cnt8 = 1;
        try8 = 1;
      }
      delay(10);  
    }
    
    cnt0 = 0, cnt1 = 0, cnt2 = 0, cnt3 = 0, cnt4 = 0, cnt5 = 0, cnt6 = 0, cnt7 = 0, cnt8 = 0, cnt9 = 0, cnt10 = 0, cnt11 = 0;
    try0 = 0, try1 = 0, try2 = 0, try3 = 0, try4 = 0, try5 = 0, try6 = 0, try7 = 0, try8 = 0, try9 = 0, try10 = 0, try11 = 0;
    
    Motor_OFF();
  }
}

void setup() {
  TCCR0B = TCCR0B & B11111000 | B00000010;
  TCCR3B = TCCR3B & B11111000 | B00000010;
  
  // 모터
  pinMode(VEL, OUTPUT);
  pinMode(RD, OUTPUT);
  pinMode(LR, OUTPUT);
  digitalWrite(VEL, LOW);
  digitalWrite(RD, HIGH);
  digitalWrite(LR, HIGH);
  // 릴레이
  pinMode(RELAY1, OUTPUT);
  pinMode(RELAY2, OUTPUT);
  pinMode(RELAY3, OUTPUT);
  pinMode(RELAY4, OUTPUT);
  pinMode(RELAY5, OUTPUT);
  pinMode(RELAY6, OUTPUT);
  digitalWrite(RELAY1, LOW);  //Relay type 'LOW' : COM to NO
  digitalWrite(RELAY2, LOW);
  digitalWrite(RELAY3, LOW);
  digitalWrite(RELAY4, LOW);
  digitalWrite(RELAY5, LOW);
  digitalWrite(RELAY6, LOW);
  
  Serial.begin(9600);
}

void loop(){
  char req = Serial.read();
  brocker(req);
}
