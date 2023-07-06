/*
  Project Name : SPC Linas Unmanned Pickup Box
  Subtitle : SPClinas
  Reporting date : 22.01.24
  Update date : 22.01.27
  written by Smartcube HW Staff 'HEONSEOK HA'
*/

//const int relay[12] = {30,31,32,33,34,35,36,37,38,39,40,41};  //Arduino Digital Pin
const int relay[9] = {30,31,32,33,34,35,36,37,38};

int t = 750; // t[ms] = (Blink Cycle)/2
int ref = 1;  // ref = ('1' before refresh() ; '0' after refresh())

void setup()
{
  for(int i=0; i<9; i++)
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

void refresh()
{
  for(int i=0; i<9; i++)
  {
    if(digitalRead(relay[i]) == HIGH)
    {
      digitalWrite(relay[i], LOW);
    }
  }
  
  ref = 0;
}

void relay1on()
{
  while(ref != 0)
  {
    digitalWrite(relay[0], HIGH);
    wait(t);
    digitalWrite(relay[0], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay2on()
{
  while(ref != 0)
  {
    digitalWrite(relay[1], HIGH);
    wait(t);
    digitalWrite(relay[1], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay3on()
{
  while(ref != 0)
  {
    digitalWrite(relay[2], HIGH);
    wait(t);
    digitalWrite(relay[2], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay4on()
{
  while(ref != 0)
  {
    digitalWrite(relay[3], HIGH);
    wait(t);
    digitalWrite(relay[3], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay5on()
{
  while(ref != 0)
  {
    digitalWrite(relay[4], HIGH);
    wait(t);
    digitalWrite(relay[4], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay6on()
{
  while(ref != 0)
  {
    digitalWrite(relay[5], HIGH);
    wait(t);
    digitalWrite(relay[5], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay7on()
{
  while(ref != 0)
  {
    digitalWrite(relay[6], HIGH);
    wait(t);
    digitalWrite(relay[6], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay8on()
{
  while(ref != 0)
  {
    digitalWrite(relay[7], HIGH);
    wait(t);
    digitalWrite(relay[7], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay9on()
{
  while(ref != 0)
  {
    digitalWrite(relay[8], HIGH);
    wait(t);
    digitalWrite(relay[8], LOW);
    wait(t);    
  }
  ref = 1;
}

/*
void relay10on()
{
  while(ref != 0)
  {
    digitalWrite(relay[9], HIGH);
    wait(t);
    digitalWrite(relay[9], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay11on()
{
  while(ref != 0)
  {
    digitalWrite(relay[10], HIGH);
    wait(t);
    digitalWrite(relay[10], LOW);
    wait(t);    
  }
  ref = 1;
}

void relay12on()
{
  while(ref != 0)
  {
    digitalWrite(relay[11], HIGH);
    wait(t);
    digitalWrite(relay[11], LOW);
    wait(t);    
  }
  ref = 1;
}
*/

void brocker(char req)
{
  if (req == 'A')
  {
    relay1on();
  }
  else if (req == 'B')
  {
    relay2on();
  }
  else if (req == 'C')
  {
    relay3on();
  }
  else if (req == 'D')
  {
    relay4on();
  }
  else if (req == 'E')
  {
    relay5on();
  }
  else if (req == 'F')
  {
    relay6on();
  }
  else if (req == 'G')
  {
    relay7on();
  }
  else if (req == 'H')
  {
    relay8on();
  }
  else if (req == 'I')
  {
    relay9on();
  }
  /*
  else if (req == 'J')
  {
    relay10on();
  }
  else if (req == 'K')
  {
    relay11on();
  }
  else if (req == 'L')
  {
    relay12on();
  }
  */
  else if (req == 'Z')
  {
    refresh();
  }
}

void loop() {
  char req = Serial.read();
  brocker(req);
}
