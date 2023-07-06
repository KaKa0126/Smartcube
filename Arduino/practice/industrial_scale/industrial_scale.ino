int key=0, SerFlag=0;
char RecPacket[11]={0, }, SenPacket[11]={0, }, Send = "3";

//패킷 수신
void RecievePacket(){
  if(Serial.available()<=0)
    return;

  char ch = Serial.read();
  if(ch==(byte)0x02||RecPacket[0]==(byte)0x02){
    RecPacket[key] = ch;
    key++;
  }
  if(key==11){
    SerFlag = 1;
    key = 0;
  }
}

void SendPacket_weight(){
  Serial.write(Send);
  SerFlag = 2;
}

void setup() {
  Serial.begin(9600);
}
void loop() {
  Serial.write("3");
  while(Serial.available()>0){
    char ch = Serial.read();
    Serial.write(ch);
  }
  delay(1000);
}
