float max_current = 0;

void setup() {
  Serial.begin(9600); //Start Serial Monitor to display current read value on Serial monitor
}

void loop() {
  unsigned int x = 0;
  float AcsValue = 0.0, Samples = 0.0, AvgAcs = 0.0, AcsValueF = 0.0;

  for (int x = 0; x < 4; x++){      //Get 4 samples
    AcsValue = analogRead(A0);      //Read current sensor values
    Samples = Samples + AcsValue;   //Add samples together
    delay (3);                      // let ADC settle before next sample 3ms
  }  
  AvgAcs=Samples/4.0;//Taking Average of Samples

  //((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
  //2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
  //out to be 2.5 which is out offset. If your arduino is working on different voltage than 
  //you must change the offset according to the input voltage)
  //0.100v(100mV) is rise in output voltage when 1A current flows at input
  
  AcsValueF = -(2.5 - (AvgAcs * (5.0 / 1024.0)) )/0.100;
  if(AcsValueF > max_current){
    max_current = AcsValueF;
  }

  Serial.print("current_current = ");
  Serial.print(AcsValueF);
  Serial.print("    max_current = ");
  Serial.println(max_current);  
  delay(10);
}
