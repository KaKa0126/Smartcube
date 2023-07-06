/*
 * ATmega2560 PWM frequency change Test
 * 
 * - Default -
 * 4,13 pin PWM frequency = 1[kHz]
 * others = 500[Hz]
 * 
 * 참고 사이트
 * ATmega2560 : https://www.etechnophiles.com/how-to-change-pwm-frequency-of-arduino-mega/
 * Ardino Uno : https://www.etechnophiles.com/change-frequency-pwm-pins-arduino-uno/
 */

void setup() {
  TCCR0B = TCCR0B & B11111000 | B00000010;
  // D4, D13 for PWM frequency of 7812.50 Hz
  TCCR1B = TCCR1B & B11111000 | B00000010;
  // D11, D12 for PWM frequency of 3921.16 Hz
  TCCR2B = TCCR2B & B11111000 | B00000010;
  // D9, D10 for  PWM frequency of 3921.16 Hz
  TCCR3B = TCCR3B & B11111000 | B00000010;
  // D2, D3, D5 for PWM frequency of 3921.16 Hz
  TCCR4B = TCCR4B & B11111000 | B00000010;
  // D6, D7, D8 for PWM frequency of 3921.16 Hz
  
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(8,OUTPUT);
  pinMode(9,OUTPUT);
  pinMode(10,OUTPUT);
  pinMode(11,OUTPUT);
  pinMode(12,OUTPUT);
  pinMode(13,OUTPUT);

  Serial.begin(9600);
}

void loop() {
  for(int i = 2 ; i <= 13 ; i++){
    analogWrite(i,155);
  }
}
