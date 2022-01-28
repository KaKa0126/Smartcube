/*
  Project Name : SPC Linas Unmanned Pickup Box
  Subtitle : SPClinas
  Reporting date : 22.01.24
  Update date : 22.01.28
  written by Smartcube HW Staff 'HEONSEOK HA'
*/

const int relay[12] = {30,31,32,33,34,35,36,37,38,39,40,41};  //Arduino Digital Pin
int t = 750; // t[ms] = (Blink Cycle)/2
int ref[12] = {0};
/*
  ref[i] = 0 : COM to NC(Relay OFF)
  ref[i] = 1 : COM to NO(Relay ON(Blink))
*/

void setup()
{
  for(int i = 0; i < 12 ; i++)
  {
    pinMode(relay[i], OUTPUT);
    digitalWrite(relay[i], LOW);  //Relay type : COM to NC
  }
  
  Serial.begin(9600);
}

void wait(unsigned long duration_ms)
{
  unsigned long time_start = millis();
  unsigned long time_end = time_start;
  bool is_complete = false;
  while (!is_complete)
  {
    // message
    char req = Serial.read();
    brocker(req);

    unsigned long time_current = millis();

    if (time_current - time_start >= duration_ms)
    {
      time_end = millis();
      is_complete = true;
    }
  }
}

void off_1()
{
  digitalWrite(relay[0], LOW);
  ref[0] = 0;
}

void off_2()
{
  digitalWrite(relay[1], LOW);
  ref[1] = 0;
}

void off_3()
{
  digitalWrite(relay[2], LOW);
  ref[2] = 0;
}

void off_4()
{
  digitalWrite(relay[3], LOW);
  ref[3] = 0;
}

void off_5()
{
  digitalWrite(relay[4], LOW);
  ref[4] = 0;
}

void off_6()
{
  digitalWrite(relay[5], LOW);
  ref[5] = 0;
}

void off_7()
{
  digitalWrite(relay[6], LOW);
  ref[6] = 0;
}

void off_8()
{
  digitalWrite(relay[7], LOW);
  ref[7] = 0;
}

void off_9()
{
  digitalWrite(relay[8], LOW);
  ref[8] = 0;
}

void off_10()
{
  digitalWrite(relay[9], LOW);
  ref[9] = 0;
}

void off_11()
{
  digitalWrite(relay[10], LOW);
  ref[10] = 0;
}

void off_12()
{
  digitalWrite(relay[11], LOW);
  ref[11] = 0;
}

void brocker(char req)
{
  if (req == 'A')
  {
    ref[0] = 1;
  }
  else if (req == 'B')
  {
    ref[1] = 1;
  }
  else if (req == 'C')
  {
    ref[2] = 1;
  }
  else if (req == 'D')
  {
    ref[3] = 1;
  }
  else if (req == 'E')
  {
    ref[4] = 1;
  }
  else if (req == 'F')
  {
    ref[5] = 1;
  }
  else if (req == 'G')
  {
    ref[6] = 1;
  }
  else if (req == 'H')
  {
    ref[7] = 1;
  }
  else if (req == 'I')
  {
    ref[8] = 1;
  }
  else if (req == 'J')
  {
    ref[9] = 1;
  }
  else if (req == 'K')
  {
    ref[10] = 1;
  }
  else if (req == 'L')
  {
    ref[11] = 1;
  }
  else if (req == 'a')
  {
    off_1();
  }
  else if (req == 'b')
  {
    off_2();
  }
  else if (req == 'c')
  {
    off_3();
  }
  else if (req == 'd')
  {
    off_4();
  }
  else if (req == 'e')
  {
    off_5();
  }
  else if (req == 'f')
  {
    off_6();
  }
  else if (req == 'g')
  {
    off_7();
  }
  else if (req == 'h')
  {
    off_8();
  }
  else if (req == 'i')
  {
    off_9();
  }
  else if (req == 'j')
  {
    off_10();
  }
  else if (req == 'k')
  {
    off_11();
  }
  else if (req == 'l')
  {
    off_12();
  }
}

void loop() {
  char req = Serial.read();
  brocker(req);
  for(int i = 0 ; i < 12 ; i++)
  {
    if(ref[i] == 1)
    {
      digitalWrite(relay[i], HIGH);
    }
  }
  wait(t);
  for(int i = 0 ; i < 12 ; i++)
  {
    if(ref[i] == 1)
    {
      digitalWrite(relay[i], LOW);
    }
  }
  wait(t);
}
