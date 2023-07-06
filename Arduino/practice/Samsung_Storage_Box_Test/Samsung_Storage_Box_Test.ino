/*
  Project Name : Samsung_Storage_Box_Test
  Reporting date : 23.03.09
  Update date : 23.03.28
  written by Smartcube HW Staff 'HEONSEOK HA'
*/

void setup() {
  Serial.begin(9600);
}

void loop(){  
  Serial.print("A0    ");
  Serial.println(analogRead(A0));
  Serial.print("A1   ");
  Serial.println(analogRead(A1));
  Serial.print("A2    ");
  Serial.println(analogRead(A2));
  Serial.print("A3    ");
  Serial.println(analogRead(A3));
  Serial.print("A4    ");
  Serial.println(analogRead(A4));
  Serial.print("A5    ");
  Serial.println(analogRead(A5));
  Serial.print("A6    ");
  Serial.println(analogRead(A6));
  Serial.print("A7    ");
  Serial.println(analogRead(A7));
  Serial.print("A8    ");
  Serial.println(analogRead(A8));
  Serial.print("A9    ");
  Serial.println(analogRead(A9));
  Serial.print("A10    ");
  Serial.println(analogRead(A10));
  Serial.print("A11    ");
  Serial.println(analogRead(A11));
  

  //Serial.print("A5    ");
  //Serial.println(analogRead(A5));
  delay(3000);
}
