/*  
  Project Name : BOX25_Internal_Type
  Subtitle : Control of ON/OFF of relay 4channel using ESP8266 Wifi module in smartphone app
  Reporting date : 22.01.28
  Update date : 22.02.03
  written by Smartcube HW Staff 'HEONSEOK HA'

  1. 시스템 구성 : WAN - 공유기1 - COM1(ESP8266 Wifi module)
  2. 앱화면의 버튼을 눌러 Relay를 ON·OFF 할 수 있다.
  (PIN_RELAY1(D1 LED), PIN_RELAY2(D5 LED), PIN_RELAY3(D9 LED), PIN_RELAY4(D11 LED))
  3. 미니 전원을 연결하여 소형화 된 무선 WiFi 모듈을 구성해본다.
  http://server_ip/pin_relay1/0 이면 PIN_RELAY1(D1 LED) 를 LOW로 설정,
  http://server_ip/pin_relay1/1 이면 PIN_RELAY1(D1 LED) 를 HIGH로 설정,
  http://server_ip/pin_relay2/0 이면 PIN_RELAY2(D5 LED) 를 LOW로 설정,
  http://server_ip/pin_relay2/1 이면 PIN_RELAY2(D5 LED) 를 HIGH로 설정,
  http://server_ip/pin_relay3/0 이면 PIN_RELAY3(D9 LED) 를 LOW로 설정,
  http://server_ip/pin_relay3/1 이면 PIN_RELAY3(D9 LED) 를 HIGH로 설정,
  http://server_ip/pin_relay4/0 이면 PIN_RELAY4(D11 LED) 를 LOW로 설정,
  http://server_ip/pin_relay4/1 이면 PIN_RELAY4(D11 LED) 를 HIGH로 설정
  http://server_ip/web/0 이면 app reset(every second)
  4. 내부망에서 동작 가능하다.
  5. 외부망에서 동작은 공유기1의 포트포워딩 설정 후 가능하다.   
*/


#include <ESP8266WiFi.h>
#include "DHT.h"
#include <SoftwareSerial.h> //SoftwareSerial을 통해 블루투스와 데이터 전송

#define PIN_RELAY1 16 //D1 LED
#define PIN_RELAY2 14 //D5 LED
#define PIN_RELAY3 12 //D9 LED
#define PIN_RELAY4 13 //D11 LED
#define WEB 2 //WEB reset every second
#define PIN_DHT 0
#define DHTTYPE DHT11
DHT dht(PIN_DHT, DHTTYPE);

// 각각의 Relay 상태(ON·OFF) 변수 설정(ON = 1, OFF = 0)
int R1val = 1;  //펠티어 소자
int R2val = 0;  //미정
int R3val = 0;  //미정
int R4val = 0;  //미정
int webval = 0;  //web reset every second val
float h = 0;
float t = 0;

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
  pinMode(WEB, OUTPUT);
  digitalWrite(WEB, 0);
  // Serial 통신으로 WiFi 네트웍에 연결
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);  // WiFi 네트웍에 설정된 id와 패스워드로 접속
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
  String s1 = "HTTP/1.1 200 OK\r\n\r\n \r\n\r\nPeltier module(4~6°C) is now ";
  String s2 = "PIN_RELAY2 is now ";
  String s3 = "PIN_RELAY3 is now ";
  String s4 = "PIN_RELAY4 is now ";
  //String s5 = "WEB is now ";
  
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
    delay(1);
    if ( t > 6){
      R1val = 1;
      digitalWrite(PIN_RELAY1, R1val);
    }
    else if ( t < 4){
      R1val = 0;
      digitalWrite(PIN_RELAY1, R1val);
    }
    delay(1);
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
  else if (req.indexOf("/web/0") != -1)
    webval = 1;
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
  
  s1 += (R1val)?"ON":"OFF";   //R1val 값에 따라 HIGH 또는 LOW 응답
  s1 += "\n";
  s2 += (R2val)?"ON":"OFF";   //R2val 값에 따라 HIGH 또는 LOW 응답
  s2 += "\n";
  s3 += (R3val)?"ON":"OFF";   //R3val 값에 따라 HIGH 또는 LOW 응답
  s3 += "\n";
  s4 += (R4val)?"ON":"OFF";   //R4val 값에 따라 HIGH 또는 LOW 응답
  s4 += "\n\n";

  // 클라이언트로 응답을 보냄
  client.print(s1);
  delay(1);
  client.print(s2);
  delay(1);
  client.print(s3);
  delay(1);
  client.print(s4);
  delay(1);
  client.println("Real Time");
  delay(1);
  client.print("Temperature: ");
  delay(1);
  client.print(t);
  delay(1);
  client.print("°C  Humidity: ");
  delay(1);
  client.print(h);
  delay(1);
  client.println("%");
  delay(1);
  Serial.println("Client disconnected");  // 응답완료 후, 클라이언트를 닫기
}
