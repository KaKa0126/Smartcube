#define relay1 1
#define relay2 2
#define relay3 3
#define relay4 4
#define relay5 5
#define relay6 6
#define relay7 7
#define relay8 8
#define relay9 9
#define relay10 10
#define relay11 11
#define relay12 12

void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(relay8, OUTPUT);
  pinMode(relay9, OUTPUT);
  pinMode(relay10, OUTPUT);
  pinMode(relay11, OUTPUT);
  pinMode(relay12, OUTPUT);
  
  digitalWrite(relay1, HIGH);   //NO 연결을 베이스로 함.
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, HIGH);
  digitalWrite(relay4, HIGH);
  digitalWrite(relay5, HIGH);
  digitalWrite(relay6, HIGH);
  digitalWrite(relay7, HIGH);
  digitalWrite(relay8, HIGH);
  digitalWrite(relay9, HIGH);
  digitalWrite(relay10, HIGH);
  digitalWrite(relay11, HIGH);
  digitalWrite(relay12, HIGH);
  
  Serial.begin(9600);

  pinMode(2, INPUT_PULLUP); //눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  
  Serial.println("type Q >>> RELAYHIGH");
  Serial.println("type W >>> RELAYLOW");
  Serial.println("type E >>> switchtest");
  Serial.println("type Z >>> OPEN");
  Serial.println("type X >>> CLOSE");
  delay(20);  //스위치 초기 오류 없앰
}

void switchcheck() {
  switch1 = digitalRead(2);
}

void RELAYHIGH()
{
  digitalWrite(relay, HIGH);
}

void RELAYLOW()
{
  digitalWrite(relay, LOW);
}

void switchtest()
{
  Serial.println(switch1);
}

void OPEN()
{
  RELAYHIGH();
  delay(50);
  RELAYLOW();
}

void CLOSE()
{
  
}

void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'Q') {
      RELAYHIGH();
    }
    else if (input == 'W') {
      RELAYLOW();
    }
    else if (input == 'E') {
      switchtest();
    }
    else if (input == 'Z') {
      OPEN();
    }
    else if (input == 'Z') {
      CLOSE();
    }
  }
}
