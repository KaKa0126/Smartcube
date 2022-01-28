/*
  Project Name : BOX25 Internal Type
  Subtitle : LED_Relay_Test
  Reporting date : 22.01.26
  Update date : 22.01.27
  written by Smartcube HW Staff 'HEONSEOK HA'
  
  Function : LED flashing + relay illuminated
  PCB : ESP12F_Relay_X4
*/

#define PIN_LED   5   //D14 LED
#define PIN_RELAY1 16 //D1 LED
#define PIN_RELAY2 14 //D5 LED
#define PIN_RELAY3 12 //D9 LED
#define PIN_RELAY4 13 //D11 LED

void setup() 
{
 pinMode(PIN_LED, OUTPUT);
 pinMode(PIN_RELAY1, OUTPUT);
 pinMode(PIN_RELAY2, OUTPUT);
 pinMode(PIN_RELAY3, OUTPUT);
 pinMode(PIN_RELAY4, OUTPUT);
 digitalWrite(PIN_LED,HIGH);      //LED OFF
 digitalWrite(PIN_RELAY1,LOW );   //RELAY1 OFF(COM to NC)
 digitalWrite(PIN_RELAY2,LOW );   //RELAY2 OFF
 digitalWrite(PIN_RELAY3,LOW );   //RELAY3 OFF
 digitalWrite(PIN_RELAY4,LOW );   //RELAY4 OFF
}

void loop() 
{
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_RELAY1,HIGH );
  delay(1000);
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(PIN_RELAY2,HIGH );       
  delay(1000);
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_RELAY3,HIGH );       
  delay(1000);
  digitalWrite(PIN_LED, HIGH);
  digitalWrite(PIN_RELAY4,HIGH );       
  delay(1000)
  
  
  digitalWrite(PIN_LED, LOW);
  digitalWrite(PIN_RELAY1,LOW );
  digitalWrite(PIN_RELAY2,LOW );
  digitalWrite(PIN_RELAY3,LOW );
  digitalWrite(PIN_RELAY4,LOW );
  delay(1000);
  digitalWrite(PIN_LED, HIGH);
  delay(1000);
}
