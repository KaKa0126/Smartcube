#define relay1 2

void setup() {
  pinMode(relay1, OUTPUT);

  //NC 연결을 베이스로 함.
  digitalWrite(relay1, LOW);
  
  Serial.begin(9600);
  
  Serial.println("type T >>> switchtest");
  Serial.println("type O >>> OPEN");
  Serial.println("type C >>> CLOSE");
  delay(20);  //스위치 초기 오류 없앰
}

void OPEN()
{
  digitalWrite(relay1, HIGH);
}

void CLOSE()
{
  digitalWrite(relay1, LOW);
}

void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'O') {
      OPEN();
    }
    else if (input == 'C') {
      CLOSE();
    }
  }
}
