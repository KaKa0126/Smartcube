int ENA = 10;
int IN1 = 22;
int IN2 = 23;
int PWM = 12;
int relay = 26;
int FG = 8;
/*
  relay 시계방향 : LOW, 반시계방향 : HIGH

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);   >>> ON

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);   >>> OFF
*/

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(PWM, OUTPUT);
  pinMode(relay, OUTPUT);
}

void loop() {  
  analogWrite(ENA, 255);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(relay, LOW);
  analogWrite(PWM, 30);
  
  delay(1000);  //시계방향회전 30PWM 인가

  analogWrite(PWM, 255);
  delay(1000);  //시계방향회전 255PWM 인가
}
