int ENA = 10;
int IN1 = 22;
int IN2 = 23;
int relay = 26;
/*
  relay 시계방향 : LOW, 반시계방향 : HIGH
  
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);   >>> ON

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);   >>> OFF 
 */

void setup() {
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(relay,OUTPUT);
}

void loop() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  
  digitalWrite(relay, LOW);
  for(int i = 10 ; i <= 255 ; i+=10){
    analogWrite(ENA, i);
    delay(400);     
  }   //시계방향회전 가속
 
  digitalWrite(relay, LOW);
  for(int i = 255 ; i >= 10 ; i-=10){
    analogWrite(ENA, i);
    delay(400);
  }
  delay(2000);  //시계방향회전 감속

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delay(2000);  // 정지
}
