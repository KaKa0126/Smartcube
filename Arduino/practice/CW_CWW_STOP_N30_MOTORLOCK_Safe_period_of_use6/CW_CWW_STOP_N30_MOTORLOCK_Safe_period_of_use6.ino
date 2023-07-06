#include <MsTimer2.h>  //실시간으로 스위치 상태 체크하기위한 라이브러리
#include <Wire.h>

#define ENA 10
#define ENB 11
#define IN1 30
#define IN2 31
#define IN3 40
#define IN4 41

int i;
int a = 255; //255

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

  Serial.begin(115200);

  MsTimer2::set(20, swichcheck); //20ms마다 스위치 상태 측정하겠다.
  MsTimer2::start();

  pinMode(2, INPUT_PULLUP); //눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  pinMode(3, INPUT_PULLUP);

  Serial.print("type Q >>> CW\n");
  Serial.print("type W >>> CCW\n");
  Serial.print("type E >>> STOP\n");
  Serial.print("type O >>> OPEN\n");
  Serial.print("type P >>> CLOSE\n");
  Serial.print("type T >>> TEST\n");
  Serial.print("type V >>> CLOSEtoPAUSE\n");
  Serial.print("type B >>> PAUSEtoOPEN\n");
  Serial.print("type N >>> OPENtoPAUSE\n");
  Serial.print("type M >>> CLOSE\n");
}

void swichcheck() {
  switch1 = digitalRead(2);
  switch2 = digitalRead(3);
}

void TEST()
{
  int cnt = 0;

  while (cnt < 3000) {
    OPEN();
    delay(5000);
    CLOSE();
    STOP();

    cnt = cnt + 1;
    Serial.println();
    Serial.print("cnt = ");
    Serial.println(cnt);
    Serial.println();
    Serial.println();

    delay(5000);
  }
}

void CLOSEtoPAUSE()
{
  CW();
  delay(200); //SW1 초기 0 -> 1 탈출시간

  while (switch2 == 1) {
    Serial.print("");
  }
  delay(d);

  STOP();
}

void PAUSEtoOPEN()
{
  CW();
  delay(200);
  while (switch2 == 1) {
    Serial.print("");
  }
  while (switch2 == 0) {
    Serial.print("");
  }

  STOP();
}

void OPENtoPAUSE()
{
  CCW();
  delay(500); //스위치 에러 없앰.

  while (switch2 == 1) {
    Serial.print("");
  }

  STOP();
}

void OPENtoPAUSEblock()
{
  CCW();

  while (switch2 == 1) {
    Serial.print("");
  }

  STOP();
}

void OPEN()
{
  int cnt = 1;  //문이 막힌 횟수 카운트

  CW();
  delay(200); //SW1 초기 0 -> 1 탈출시간

  while (switch2 == 1) {
    Serial.print("");
  }
  delay(d);
  while (switch2 == 0) {
    Serial.print("");
  }
  delay(d);

  while (switch2 == 1) {
    char input;

    while (1) {
      input = Serial.read();

      if (input == 'Z' && cnt < 3) {
        cnt += 1;
        OPENtoPAUSEblock();
        delay(d);
        CW();
        delay(200);
      } //이상신호 'Z'가 들어오면 대기상태까지 CCW 후에, CW로 문 밀어주기
      else if (input == 'Z' && cnt == 3) {
        OPENtoPAUSEblock();
        break;
      } //3번 못 밀면 대기상태로
      else if (switch2 == 0) {
        STOP();
        break;
      } //3번안에 정상작동
    }
    break;
  }
}

void CLOSE()
{
  delay(d);
  CCW();
  delay(d);
  while (switch1 == 0) {
    Serial.print("");
  }
  delay(d);
  while (switch1 == 1) {
    Serial.print("");
  }
  delay(60);  //정확히 잠기게 해주기 위한 딜레이

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
    else if (input == 'O') {
      OPEN();
    }
    else if (input == 'P') {
      CLOSE();
    }
    else if (input == 'V') {
      CLOSEtoPAUSE();
    }
    else if (input == 'B') {
      PAUSEtoOPEN();
    }
    else if (input == 'N') {
      OPENtoPAUSE();
    }
    else if (input == 'M') {
      CLOSE();
    }
    else if (input == 'T') {
      TEST();
    }
  }
}
