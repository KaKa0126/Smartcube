#define relay1 1
#define relay2 2
#define relay3 3
int switch1,switch2;

void setup() {
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  pinMode(relay3, OUTPUT);

  //NC 연결을 베이스로 함.
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  
  Serial.begin(9600);

  //눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  
  Serial.println("type T >>> switchtest");
  Serial.println("type O >>> OPEN");
  Serial.println("type C >>> CLOSE");
  delay(20);  //스위치 초기 오류 없앰
}


void switchcheck() {
  switch1 = digitalRead(4);
  switch2 = digitalRead(5);
}

void switchtest()
{
  Serial.println(switch1);
  Serial.println(switch2);
}

void OPEN()
{
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  digitalWrite(relay3, LOW);
  delay(300);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, HIGH);
}

void CLOSE()
{
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
}

void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'T') {
      switchtest();
    }
    else if (input == 'O') {
      OPEN();
    }
    else if (input == 'C') {
      CLOSE();
    }
  }
}
