void setup() {  
  Serial.begin(9600);
}

void loop() {
  String input;
  
  while(Serial.available()){
    input = Serial.readStringUntil('\n');
    Serial.println(input);
  }
}
