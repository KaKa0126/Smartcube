#define ENA 10
#define ENB 11
#define IN1 30
#define IN2 31
#define IN3 40
#define IN4 41

int a = 255;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);  
  Serial.print("type T >>> TEST\n");
}

void TEST()
{
  int cnt = 0;
  
  while(cnt < 1000){
    CW();
    delay(3000);
    STOP();
    delay(1000);
    CCW();
    delay(1000);
    STOP();
    delay(1000);
    CCW();
    delay(2000);
    STOP();
    delay(5000);

    cnt = cnt + 1;
    Serial.println(cnt);
  }
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

    if (input == 'T') {
      TEST();
    }
  }
}
