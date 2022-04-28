/*  
  Project Name : BOX25_Internal_Type
  Subtitle : Control of ON/OFF of relay 4channel using ESP8266 Wifi module in smartphone app
  Reporting date : 22.01.28
  Update date : 22.02.11
  written by Smartcube HW Staff 'HEONSEOK HA'

  1. 시스템 구성 : WAN - 공유기1 - COM1(ESP8266 Wifi module)
  2. 앱화면의 버튼을 눌러 Relay를 ON·OFF 할 수 있다.
  (PIN_RELAY1(D1 LED), PIN_RELAY2(D5 LED), PIN_RELAY3(D9 LED), PIN_RELAY4(D11 LED))
  3. 미니 전원을 연결하여 소형화 된 무선 WiFi 모듈을 구성해본다.
  app에서 'Start_COMM' 누르면 통신 시작, 'End_COMM' 누르면 통신 중단,
  http://server_ip/pin_relay1/0 이면 PIN_RELAY1(D1 LED) 를 LOW로 설정,
  http://server_ip/pin_relay1/1 이면 PIN_RELAY1(D1 LED) 를 HIGH로 설정,
  http://server_ip/pin_relay2/0 이면 PIN_RELAY2(D5 LED) 를 LOW로 설정,
  http://server_ip/pin_relay2/1 이면 PIN_RELAY2(D5 LED) 를 HIGH로 설정,
  http://server_ip/pin_relay3/0 이면 PIN_RELAY3(D9 LED) 를 LOW로 설정,
  http://server_ip/pin_relay3/1 이면 PIN_RELAY3(D9 LED) 를 HIGH로 설정,
  http://server_ip/pin_relay4/0 이면 PIN_RELAY4(D11 LED) 를 LOW로 설정,
  http://server_ip/pin_relay4/1 이면 PIN_RELAY4(D11 LED) 를 HIGH로 설정
  4. 내부망에서 동작 가능하다.
  5. 외부망에서 동작은 공유기1의 포트포워딩 설정 후 가능하다.
*/

#include <ESP8266WiFi.h>
#include "DHT.h"

#define PIN_RELAY1 16 //D1 LED
#define PIN_RELAY2 14 //D5 LED
#define PIN_RELAY3 12 //D9 LED
#define PIN_RELAY4 13 //D11 LED
#define PIN_SWITCH 4 //EM Lock switch
#define PIN_DHT 0
#define DHTTYPE DHT11
DHT dht(PIN_DHT, DHTTYPE);

// 각각의 Relay 상태(ON·OFF) 변수 설정(ON = 1, OFF = 0)
int R1val = 1;  // Peltier
int R2val = 0;  // TBD
int R3val = 0;  // TBD
int R4val = 0;  // TBD
int EMval = 0;  // EMval = 0 : LOW(connect), EMval = HIGH(disconnect)
float h = 0;  //humidity value
float t = 0;  //temperature value

const char* ssid = "sclab_3F_2.4GHz";     // 공유기(AP) 접속 ID
const char* password = "emCMjbM8YjdCpdz"; // 공유기(AP) 접속 패스워드

// 서버 인스턴스 생성 (80번 포트 지정)
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);

  dht.begin();
  
  // PIN_RELAY 포트 초기화
  pinMode(PIN_RELAY1, OUTPUT);
  digitalWrite(PIN_RELAY1, 0);
  pinMode(PIN_RELAY2, OUTPUT);
  digitalWrite(PIN_RELAY2, 0);
  pinMode(PIN_RELAY3, OUTPUT);
  digitalWrite(PIN_RELAY3, 0);
  pinMode(PIN_RELAY4, OUTPUT);
  digitalWrite(PIN_RELAY4, 0);
  pinMode(PIN_SWITCH, INPUT_PULLUP);
  // Connect : LOW ; Disconnect : HIGH
  
  // Serial 통신으로 WiFi 네트웍에 연결
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  // 고정 IP 설정
  IPAddress ip(192, 168, 0, 71); // 사용할 IP 주소
  IPAddress gateway(192, 168, 0, 1); // 게이트웨이 주소
  IPAddress subnet(255, 255, 255, 0); // 서브넷 주소
  WiFi.config(ip, gateway, subnet);

  // WiFi 네트웍에 설정된 id와 패스워드로 접속
  WiFi.begin(ssid, password); 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // 서버 시작
  server.begin();
  Serial.println("Server started");

  // 서버의(esp8266) IP 주소를 출력
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop() {  
  // 클라이언트가 접속하는지 체크
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // 클라이언트가 데이터를 보낼 때까지 대기
  Serial.println("new client");
  while(!client.available()){
    t = dht.readTemperature();
    h = dht.readHumidity();
    EMval = digitalRead(PIN_SWITCH);
    
    if ( t > 6){
      R1val = 1;
      digitalWrite(PIN_RELAY1, R1val);
    }
    else if ( t < 4){
      R1val = 0;
      digitalWrite(PIN_RELAY1, R1val);
    }
  }
  
  // 요청 첫 출을 읽기
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // 요청 사항과 비교  
  if (req.indexOf("/pin_relay1/0") != -1)
    R1val = 0;
  else if (req.indexOf("/pin_relay1/1") != -1) 
    R1val = 1;
  else if (req.indexOf("/pin_relay2/0") != -1)
    R2val = 0;
  else if (req.indexOf("/pin_relay2/1") != -1)
    R2val = 1;
  else if (req.indexOf("/pin_relay3/0") != -1)
    R3val = 0;
  else if (req.indexOf("/pin_relay3/1") != -1)
    R3val = 1;
  else if (req.indexOf("/pin_relay4/0") != -1)
    R4val = 0;
  else if (req.indexOf("/pin_relay4/1") != -1)
    R4val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  
  // 요청에 따라 PIN_RELAY 상태(ON or OFF) 세팅
  digitalWrite(PIN_RELAY1, R1val);
  digitalWrite(PIN_RELAY2, R2val);
  digitalWrite(PIN_RELAY3, R3val);
  digitalWrite(PIN_RELAY4, R4val);

  String s0 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
  String s1 = "Peltier module(4~6°C) is now ";
  String s3 = "EM Lock is now ";
  String s4 = "PIN_RELAY4 is now ";
  
  s1 += (R1val)?"ON":"OFF";   //R1val 값에 따라 HIGH 또는 LOW 응답
  s1 += "\n";
  s3 += (R3val)?"ON":"OFF";   //R2val 값에 따라 HIGH 또는 LOW 응답
  s3 += "\n";
  s4 += (R4val)?"ON":"OFF";   //R4val 값에 따라 HIGH 또는 LOW 응답
  s4 += "\n\n";

  // 클라이언트로 응답을 보냄
  client.print(s0);
  client.print(EMval);
  client.print(",");  // 응답 분리
  client.print("\n\n"+s1);
  client.print(s3);
  client.print(s4);
  client.print("\nReal Time\n");
  client.print("Temperature: "+String(t)+"°C  Humidity: "+String(h)+"%");
  delay(1);

  // 응답완료 후, 클라이언트를 닫기
  Serial.println("Client disconnected");
}
