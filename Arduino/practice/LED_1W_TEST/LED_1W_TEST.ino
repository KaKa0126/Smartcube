void setup() {
  Serial.begin(9600);
  
  pinMode(4, OUTPUT);
}

void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();
  }

  if (input == 'Q') {
    digitalWrite(4, HIGH);
  }
  else if (input == 'W') {
    digitalWrite(4, LOW);
  }  
}
