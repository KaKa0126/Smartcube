/*
 * RC522(RF reader) + ATmega2560 pin connect information
 * SDA(SS)  : 53
 * SCK      : 52 
 * MOSI     : 51 
 * MISO     : 50
 * IRQ
 * GND      : GND
 * RST      : 9
 * 3.3V     : 3.3V
 */
#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 53
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class 

void setup() {
  Serial.begin(115200);
  SPI.begin(); 
  rfid.PCD_Init();
}

void loop() {
  if ( ! rfid.PICC_IsNewCardPresent())
    return;
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  //rfid.uid.uidByte[i]
  String tag_id;
  for(int i = 0 ; i < 4 ; i++){
    tag_id += String(rfid.uid.uidByte[i],HEX);
  }
  Serial.println(tag_id);

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}
