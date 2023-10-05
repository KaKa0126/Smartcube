/*
  Project Name : PO_Box
  Subtitle : PO_Box
  Reporting date : 23.07.19
  Update date : 23.09.05
  written by Smartcube HW Senior Staff 'HEONSEOK HA'
*/

/*
 * Relay Model : RELAY-M1(CH8), 5V
 * Arduino Model : Arduino ATmega2560
 * Arduino ATmega2560 digital pin 2, 5는 사용불가
 * 
 * SMPS 24V의 'COM' 단자와 Arduino 보드의 GND 단자를 연결시켜야 함.
 * ---> 단자 사이의 교류 전압을 제거 시켜 포토센서 감지 신호 전압을 정확하게 측정하기 위해서
 * 
 * 설계상으로 모터가 초기 위치일 때
 *  1) 'Q' 명령 시, 초기 위치에서 물품을 드랍시키는 프로세스
 *  2) 'W' 명령 시, 초기 위치로 원위치 시키는 프로세스
 */

#define relay1 3
#define relay2 4
#define relay3 6

float voltage0;
float voltage1;

void setup() {
  // put your setup code here, to run once:
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  digitalWrite(relay1, HIGH);   //NC 연결을 베이스로 함.
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);

  Serial.begin(9600);
}

void STOP() //센서 감지하여 모터 구동 멈춤
{  
  do{
    //voltage0 = voltage1 = 0[V] or 1.17[V]
    voltage0 = analogRead(A0) * (5.0 / 1023.0);
    voltage1 = analogRead(A1) * (5.0 / 1023.0);
    delay(10);
  }while(voltage0 > 0.5 && voltage1 > 0.5);
  
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
}

void FORSESTOP() //센서 감지하지 않고, 모터 구동 멈춤
{ 
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
}

void CW() //거치대 상승
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, HIGH);

  //0.5초 동안 감지된 센서에서 벗어날 시간 
  delay(50);
}

void CCW()  //거치대 하강
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, LOW);
  
  //0.5초 동안 감지된 센서에서 벗어날 시간 
  delay(50);
}

void loop() {
  // put your main code here, to run repeatedly:
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'Q') {
      CCW();
      delay(700);
      FORSESTOP(); //2023.09.05 : 거치대 하강 도중에 기구적으로 걸림 발생하여, 하단 센서 감지 하지 않고 강제로 멈춤
    }
    else if (input == 'W') {
      CW();
      STOP(); //센서 감지하여 멈춤
    }
    
    else if (input == 'E') {
      //voltage0 = voltage1 = 0[V] or 1.17[V]
      voltage0 = analogRead(A0) * (5.0 / 1023.0);
      voltage1 = analogRead(A1) * (5.0 / 1023.0);
      Serial.print("A0 : ");
      Serial.print(voltage0);
      Serial.print("      A1 : ");
      Serial.println(voltage1);
    }
  } 
}
