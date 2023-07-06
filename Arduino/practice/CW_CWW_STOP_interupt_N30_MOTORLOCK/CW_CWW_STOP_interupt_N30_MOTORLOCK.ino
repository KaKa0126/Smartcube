#include <MsTimer2.h>  //실시간으로 스위치 상태 체크하기위한 라이브러리
#include <Wire.h>

//#include <Adafruit_INA219.h>

#define ENA 10
#define ENB 11
#define IN1 30
#define IN2 31
#define IN3 40
#define IN4 41

//Adafruit_INA219 ina219;

int i;
int a = 255; //255
float rr = 1710; //정격속도[pwm 255]로 한바퀴도는데 걸리는 시간[ms] 1710
float ar = 44;  //정격속도[pwm 255]까지 가속시 한바퀴 도는데 걸리는 시간[ms]  44
float sr = 37;  //정격속도[pwm 255]까지 감속시 한바퀴 도는데 걸리는 시간[ms]  37
float rrr = 1;  //rr의 한바퀴 기준
float arr = 1;  //ar의 한바퀴 기준
float srr = 1;  //sr의 한바퀴 기준

int switch1;
int switch2;
int d = 20; //delay 스위치의 오차를 없애는 딜레이

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);

  MsTimer2::set(20, swichcheck); //20ms마다 스위치 상태 측정하겠다.
  MsTimer2::start();

  pinMode(2, INPUT_PULLUP); //눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  pinMode(3, INPUT_PULLUP);

  //uint32_t currentFrequency;

  Serial.print("type Q >>> CW\n");
  Serial.print("type W >>> CCW\n");
  Serial.print("type E >>> STOP\n");
  Serial.print("type Z >>> Speed up\n");
  Serial.print("type X >>> Speed down\n");
  Serial.print("type C >>> Speed reset\n");
  Serial.print("type O >>> OPEN\n");
  Serial.print("type P >>> CLOSE\n");
  Serial.print("type T >>> TEST\n");
  Serial.print("type M >>> MEASURE\n");


  /*
  while (!Serial) {
    // will pause Zero, Leonardo, etc until serial console opens
    delay(1);
  }
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("Measuring voltage and current with INA219 ...");
  */
}
/*
void syscheck()
{
  float current_mA = 0;
  current_mA = ina219.getCurrent_mA();
  if (current_mA > 750) {
    STOP();
  }
}
//과부하시(0.75A 초과시) 일단 멈추기, 코드가 함수 내에 갖힌 상태로 끝나므로, reset버튼 누르기.
*/
/*
void measure(){
  float shuntvoltage = 0;
  float busvoltage = 0;
  float current_mA = 0;
  float loadvoltage = 0;
  float power_mW = 0;

  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.println("");
}
*/

void swichcheck() {
  switch1 = digitalRead(2);
  switch2 = digitalRead(3);
}

void TEST()
{
  Serial.println(switch1);
  Serial.println(switch2);
  CW();
  delay(3000);
  STOP();
}

void OPEN()
{
  CW();
  delay(200); //SW1 초기 0 -> 1 탈출시간

  while (switch2 == 1) {
    //syscheck();
  }
  delay(d);
  while (switch2 == 0) {
    //syscheck();
  }
  delay(d);
  while (switch2 == 1) {
    //syscheck();
  }

  delay(d);
  CCW();
  delay(200); //스위치 에러 없앰.

  while (switch2 == 1) {
    //syscheck();
  }

  STOP();
}

void CLOSE()
{
  delay(d);
  CCW();
  delay(d);
  while (switch1 == 0) {
    //syscheck();
  }
  delay(d);
  while (switch1 == 1) {
    //syscheck();
  }
  delay(60);  //마무리를 정확한 위치로 가게 해주는 딜레이.

  STOP();
}

void CW()
{
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, a);
  analogWrite(ENB, a);
}

void CCW()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  analogWrite(ENA, a);
  analogWrite(ENB, a);
}

void STOP()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void suddenSTOP()
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void SpeedUP() {
  STOP();
  if (a <= 230) {
    a += 25;
  }
  else {
    a = 255;
  }
}

void SpeedDOWN() {
  STOP();
  if (a >= 25) {
    a -= 25;
  }
  else {
    a = 0;
  }
}

void SpeedRESET() {
  STOP();
  a = 255;
}

void loop() {
  char input;

  while (Serial.available()) {
    input = Serial.read();

    if (input == 'Q') {
      CW();
    }
    else if (input == 'W') {
      CCW();
    }
    else if (input == 'E') {
      STOP();
    }
    else if (input == 'Z') {
      SpeedUP();
    }
    else if (input == 'X') {
      SpeedDOWN();
    }
    else if (input == 'C') {
      SpeedRESET();
    }
    else if (input == 'O') {
      OPEN();
    }
    else if (input == 'P') {
      CLOSE();
    }
    else if (input == 'T') {
      TEST();
    }
    /*
    else if (input == 'M') {
      measure();
    }
    */
  }
}
