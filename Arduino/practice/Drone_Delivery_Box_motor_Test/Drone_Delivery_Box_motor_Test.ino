//VEL = SCL+SCR : 속도 조절
#define VEL1 2
#define VEL2 3
#define VEL3 4
#define VEL4 5
#define VEL5 6
#define VEL6 7
#define VEL7 8
#define VEL8 9
#define RD1 22
#define LS1 23
#define RD2 24
#define LS2 25
#define RD3 26
#define LS3 27
#define RD4 28
#define LS4 29
#define RD5 30
#define LS5 31
#define RD6 32
#define LS6 33
#define RD7 34
#define LS7 35
#define RD8 36
#define LS8 37

//int slow_velocity = 0;

void setup() {
  pinMode(VEL1, OUTPUT);
  pinMode(VEL2, OUTPUT);
  pinMode(VEL3, OUTPUT);
  pinMode(VEL4, OUTPUT);
  pinMode(VEL5, OUTPUT);
  pinMode(VEL6, OUTPUT);
  pinMode(VEL7, OUTPUT);
  pinMode(VEL8, OUTPUT);
  pinMode(RD1, OUTPUT);
  pinMode(LS1, OUTPUT);
  pinMode(RD2, OUTPUT);
  pinMode(LS2, OUTPUT);
  pinMode(RD3, OUTPUT);
  pinMode(LS3, OUTPUT);
  pinMode(RD4, OUTPUT);
  pinMode(LS4, OUTPUT);
  pinMode(RD5, OUTPUT);
  pinMode(LS5, OUTPUT);
  pinMode(RD6, OUTPUT);
  pinMode(LS6, OUTPUT);
  pinMode(RD7, OUTPUT);
  pinMode(LS7, OUTPUT);
  pinMode(RD8, OUTPUT);
  pinMode(LS8, OUTPUT);

  //초기 셋팅
  digitalWrite(VEL1, LOW);
  digitalWrite(VEL2, LOW);
  digitalWrite(VEL3, LOW);
  digitalWrite(VEL4, LOW);
  digitalWrite(VEL5, LOW);
  digitalWrite(VEL6, LOW);
  digitalWrite(VEL7, LOW);
  digitalWrite(VEL8, LOW);
  digitalWrite(RD1, HIGH);
  digitalWrite(LS1, HIGH);
  digitalWrite(RD2, HIGH);
  digitalWrite(LS2, HIGH);
  digitalWrite(RD3, HIGH);
  digitalWrite(LS3, HIGH);
  digitalWrite(RD4, HIGH);
  digitalWrite(LS4, HIGH);
  digitalWrite(RD5, HIGH);
  digitalWrite(LS5, HIGH);
  digitalWrite(RD6, HIGH);
  digitalWrite(LS6, HIGH);
  digitalWrite(RD7, HIGH);
  digitalWrite(LS7, HIGH);
  digitalWrite(RD8, HIGH);
  digitalWrite(LS8, HIGH);
    
  Serial.begin(9600);
  
  Serial.println("type A >>> CW1");
  Serial.println("type a >>> CCW1");
  Serial.println("type 1 >>> OFF1");
  Serial.println(".");
  Serial.println(".");
  Serial.println(".");
  Serial.println("type H >>> CW8");
  Serial.println("type h >>> CCW8");
  Serial.println("type 8 >>> OFF8");
}

/*
void SLOWCW()
{
  analogWrite(VEL1, slow_velocity);
  digitalWrite(RD1, LOW);
  digitalWrite(LS1, LOW);
}

void SLOWCCW()
{
  analogWrite(VEL1, slow_velocity);
  digitalWrite(RD1, HIGH);
  digitalWrite(LS1, LOW);
}
*/

void CW1()
{
  digitalWrite(VEL1, HIGH);
  digitalWrite(RD1, LOW);
  digitalWrite(LS1, LOW);
}

void CW2()
{
  digitalWrite(VEL2, HIGH);
  digitalWrite(RD2, LOW);
  digitalWrite(LS2, LOW);
}

void CW3()
{
  digitalWrite(VEL3, HIGH);
  digitalWrite(RD3, LOW);
  digitalWrite(LS3, LOW);
}

void CW4()
{
  digitalWrite(VEL4, HIGH);
  digitalWrite(RD4, LOW);
  digitalWrite(LS4, LOW);
}

void CW5()
{
  digitalWrite(VEL5, HIGH);
  digitalWrite(RD5, LOW);
  digitalWrite(LS5, LOW);
}

void CW6()
{
  digitalWrite(VEL6, HIGH);
  digitalWrite(RD6, LOW);
  digitalWrite(LS6, LOW);
}

void CW7()
{
  digitalWrite(VEL7, HIGH);
  digitalWrite(RD7, LOW);
  digitalWrite(LS7, LOW);
}

void CW8()
{
  digitalWrite(VEL8, HIGH);
  digitalWrite(RD8, LOW);
  digitalWrite(LS8, LOW);
}

void CCW1()
{
  digitalWrite(VEL1, HIGH);
  digitalWrite(RD1, HIGH);
  digitalWrite(LS1, LOW);
}

void CCW2()
{
  digitalWrite(VEL2, HIGH);
  digitalWrite(RD2, HIGH);
  digitalWrite(LS2, LOW);
}

void CCW3()
{
  digitalWrite(VEL3, HIGH);
  digitalWrite(RD3, HIGH);
  digitalWrite(LS3, LOW);
}

void CCW4()
{
  digitalWrite(VEL4, HIGH);
  digitalWrite(RD4, HIGH);
  digitalWrite(LS4, LOW);
}

void CCW5()
{
  digitalWrite(VEL5, HIGH);
  digitalWrite(RD5, HIGH);
  digitalWrite(LS5, LOW);
}

void CCW6()
{
  digitalWrite(VEL6, HIGH);
  digitalWrite(RD6, HIGH);
  digitalWrite(LS6, LOW);
}

void CCW7()
{
  digitalWrite(VEL7, HIGH);
  digitalWrite(RD7, HIGH);
  digitalWrite(LS7, LOW);
}

void CCW8()
{
  digitalWrite(VEL8, HIGH);
  digitalWrite(RD8, HIGH);
  digitalWrite(LS8, LOW);
}

void OFF1()
{
  digitalWrite(VEL1, LOW);
  digitalWrite(RD1, HIGH);
  digitalWrite(LS1, HIGH);
}

void OFF2()
{
  digitalWrite(VEL2, LOW);
  digitalWrite(RD2, HIGH);
  digitalWrite(LS2, HIGH);
}

void OFF3()
{
  digitalWrite(VEL3, LOW);
  digitalWrite(RD3, HIGH);
  digitalWrite(LS3, HIGH);
}

void OFF4()
{
  digitalWrite(VEL4, LOW);
  digitalWrite(RD4, HIGH);
  digitalWrite(LS4, HIGH);
}

void OFF5()
{
  digitalWrite(VEL5, LOW);
  digitalWrite(RD5, HIGH);
  digitalWrite(LS5, HIGH);
}

void OFF6()
{
  digitalWrite(VEL6, LOW);
  digitalWrite(RD6, HIGH);
  digitalWrite(LS6, HIGH);
}

void OFF7()
{
  digitalWrite(VEL7, LOW);
  digitalWrite(RD7, HIGH);
  digitalWrite(LS7, HIGH);
}

void OFF8()
{
  digitalWrite(VEL8, LOW);
  digitalWrite(RD8, HIGH);
  digitalWrite(LS8, HIGH);
}

void loop() {
  char input;
  
  while(Serial.available()){
    input = Serial.read();

    if (input == 'A') {
      CW1();
    }
    else if (input == 'a') {
      CCW1();
    }
    else if (input == '1') {
      OFF1();
    }
    else if (input == 'B') {
      CW2();
    }
    else if (input == 'b') {
      CCW2();
    }
    else if (input == '2') {
      OFF2();
    }
     else if (input == 'C') {
      CW3();
    }
    else if (input == 'c') {
      CCW3();
    }
    else if (input == '3') {
      OFF3();
    }
     else if (input == 'D') {
      CW4();
    }
    else if (input == 'd') {
      CCW4();
    }
    else if (input == '4') {
      OFF4();
    }
     else if (input == 'E') {
      CW5();
    }
    else if (input == 'e') {
      CCW5();
    }
    else if (input == '5') {
      OFF5();
    }
     else if (input == 'F') {
      CW6();
    }
    else if (input == 'f') {
      CCW6();
    }
    else if (input == '6') {
      OFF6();
    }
     else if (input == 'G') {
      CW7();
    }
    else if (input == 'g') {
      CCW7();
    }
    else if (input == '7') {
      OFF7();
    }
     else if (input == 'H') {
      CW8();
    }
    else if (input == 'h') {
      CCW8();
    }
    else if (input == '8') {
      OFF8();
    }
  }
}
