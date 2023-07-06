void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  static int cnt = 10;

  Serial.println(cnt);
  cnt++;

  delay(1000);
}
