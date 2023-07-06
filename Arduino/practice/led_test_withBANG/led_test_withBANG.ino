#define LED 26


String msg = "Hello, World!";

void wait(unsigned long duration_ms)
{
  unsigned long time_start = millis();
  unsigned long time_end = time_start;
  bool is_complete = false;
  while (!is_complete)
  {
    // 메시지 처리
    char req = Serial.read();
    brocker(req);

    unsigned long time_current = millis();

    if (time_current - time_start >= duration_ms)
    {
      time_end = millis();
      is_complete = true;
    }
  }
}


int switch0;  // Telescopic_drive_base에 부착되어, 함이 닫혔을 때, 눌린상태(0) 값 유지

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(9600);

  pinMode(6, INPUT_PULLUP); //switch0;눌리지 않은 상태(기본 상태)에서 '1', 눌린 상태에서 '0' 출력
}

void loop() {
  char req = Serial.read();
  brocker(req);
}

void job_Q()
{
  // 1
  
  digitalWrite(LED, HIGH);  // 모터 동작
  msg = "0,Q,1";

  // 2
  
  wait(5000);

  // 3

  digitalWrite(LED, LOW);  // 모터 정지
  msg = "0,Q,0";
}

void job_W()
{
  // 1
  
  digitalWrite(LED, HIGH);  // 모터 동작
  msg = "0,W,1";

  // 2
  
  while(switch0 == 1)
  {
    Serial.print("");
  }

  // 3

  digitalWrite(LED, LOW);  // 모터 정지
  msg = "0,W,0";
}

void brocker(char req)
{
  if (req == '?')
  {
    Serial.println(msg);
  }
  else if (req == '!')
  {
    digitalWrite(LED, HIGH);
    delay(100);
    digitalWrite(LED, LOW);
  }
  else if (req == 'Q')
  {
    job_Q();
    Serial.println(msg);
  }
  else if (req == 'W')
  {
    job_W();
    Serial.println(msg);
  }
}
