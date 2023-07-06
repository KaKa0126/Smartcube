//T1 동작
int ENA = 10; //Speed Control Pin
int IN1 = 22;
int IN2 = 23;
/*
  relay 시계방향 : LOW, 반시계방향 : HIGH

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);   >>> ON

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);   >>> OFF
*/

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
}

void loop() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 250);
  delay(3000);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  analogWrite(ENA, 250);
  delay(3000);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  analogWrite(ENA, 250);
  delay(3000);

}
