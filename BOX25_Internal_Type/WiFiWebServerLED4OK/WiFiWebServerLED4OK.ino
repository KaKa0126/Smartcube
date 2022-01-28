/*  ### 스마트폰 WiFi로 esp8266모듈회로의 LED 켜고·끄기 ### 
 *  1. LED를 연결한 esp8266 회로를 서버로 동작시킨다. 
 *  2. 앱인벤터를 이용하여 접속앱을 직접 제작 한다.
 *  3. 접속앱에서 esp8266 서버 주소를 입력하고 접속한다. 
 *    (접속 IP 주소는 코드 업로드가 완료되면 시리얼모니터(115200bps)를 통해 확인 가능함)
 *  4. 앱화면의 버튼을 눌러 LED를 On·Off 할 수 있다.(PIN_RELAY1(D1 LED), PIN_RELAY2(D5 LED), PIN_RELAY3(D9 LED), PIN_RELAY4(D11 LED))
 *  5. 아두이노 우노를 제거하고 미니 전원을 연결하여 소형화 된 무선 WiFi 모듈을 구성해본다.
 *    http://server_ip/gpio1/0 이면 PIN_RELAY1(D1 LED) 를 LOW로 설정,
 *    http://server_ip/gpio1/1 이면 PIN_RELAY1(D1 LED) 를 HIGH로 설정,
 *    http://server_ip/gpio2/0 이면 PIN_RELAY2(D5 LED) 를 LOW로 설정,
 *    http://server_ip/gpio2/1 이면 PIN_RELAY2(D5 LED) 를 HIGH로 설정,
 *    http://server_ip/gpio3/0 이면 PIN_RELAY3(D9 LED) 를 LOW로 설정,
 *    http://server_ip/gpio3/1 이면 PIN_RELAY3(D9 LED) 를 HIGH로 설정,
 *    http://server_ip/gpio4/0 이면 PIN_RELAY4(D11 LED) 를 LOW로 설정,
 *    http://server_ip/gpio4/1 이면 PIN_RELAY4(D11 LED) 를 HIGH로 설정
 *    
 *  주의 : 반드시 동일한 내부 네트웍(하나의 공유기로 연결된)에서 동작 시켜야 접속이 이루어 집니다. 
 *  
 *  제작과정의 좀더 상세한 내용과 설명 영상은 http://rasino.tistory.com/300 참조 하세요 
 */

#include <ESP8266WiFi.h>

#define PIN_RELAY1 16 //D1 LED
#define PIN_RELAY2 14 //D5 LED
#define PIN_RELAY3 12 //D9 LED
#define PIN_RELAY4 13 //D11 LED
/*
#define GPIO0 0
#define GPIO2 2
*/

const char* ssid = "sclab_3F_2.4GHz";     // 공유기(AP) 접속 ID
const char* password = "emCMjbM8YjdCpdz"; // 공유기(AP) 접속 패스워드
// 서버 인스턴스를 생성합니다 (80번 포트 지정)
WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  delay(10);
  // GPIO 포트들을 초기화 합니다
  pinMode(PIN_RELAY1, OUTPUT);
  digitalWrite(PIN_RELAY1, 0);
  pinMode(PIN_RELAY2, OUTPUT);
  digitalWrite(PIN_RELAY2, 0);
  pinMode(PIN_RELAY3, OUTPUT);
  digitalWrite(PIN_RELAY3, 0);
  pinMode(PIN_RELAY4, OUTPUT);
  digitalWrite(PIN_RELAY4, 0);
  // Serial 통신으로 WiFi 네트웍에 연결합니다.
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);  // 네트웍에 설정된 id와 패스워드로 접속을 합니다.
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // 서버 시작
  server.begin();
  Serial.println("Server started");

  // 서버의(esp8266) IP 주소를 출력합니다 
  Serial.println(WiFi.localIP());
}

void loop() {
  // 클라이언트가 접속하는지 체크 합니다
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  
  // 클라이언트가 데이터를 보낼 때까지 기다립니다.
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }
  
  // 요청 첫 출을 읽어 옵니다.
  String req = client.readStringUntil('\r');
  Serial.println(req);
  client.flush();
  
  // 요청 사항과 비교해봅니다.
  int R1val=0;
  int R2val=0;
  int R3val=0;
  int R4val=0;
  
  if (req.indexOf("/gpio1/0") != -1)
    R1val = 0;
  else if (req.indexOf("/gpio1/1") != -1) 
    R1val = 1;
  else if (req.indexOf("/gpio2/0") != -1)
    R2val = 0;
  else if (req.indexOf("/gpio2/1") != -1)
    R2val = 1;
  else if (req.indexOf("/gpio3/0") != -1)
    R3val = 0;
  else if (req.indexOf("/gpio3/1") != -1)
    R3val = 1;
  else if (req.indexOf("/gpio4/0") != -1)
    R4val = 0;
  else if (req.indexOf("/gpio4/1") != -1)
    R4val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  // 요청에 따라 GPIO 들을 셋팅합니다(ON 또는 OFF)
  digitalWrite(PIN_RELAY1, R1val);
  digitalWrite(PIN_RELAY2, R2val);
  digitalWrite(PIN_RELAY3, R3val);
  digitalWrite(PIN_RELAY4, R4val);
  /*
  int GP0val=0;
  int GP2val=0;
  
  if (req.indexOf("/gpio0/0") != -1)
    GP0val = 0;
  else if (req.indexOf("/gpio0/1") != -1) 
    GP0val = 1;
  else if (req.indexOf("/gpio2/0") != -1)
    GP2val = 0;
  else if (req.indexOf("/gpio2/1") != -1)
    GP2val = 1;
  else {
    Serial.println("invalid request");
    client.stop();
    return;
  }
  // 요청에 따라 GPIO 들을 셋팅합니다(ON 또는 OFF)
  digitalWrite(GPIO0, GP0val);
  digitalWrite(GPIO2, GP2val);  
  */
  
  client.flush();
  // 응답을 준비합니다.
  String s1 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\nPIN_RELAY1 is now ";
  String s2 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\nPIN_RELAY2 is now ";
  String s3 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\nPIN_RELAY3 is now ";
  String s4 = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n \r\n\r\nPIN_RELAY4 is now ";
  s1 += (R1val)?"HIGH":"LOW";   //R1val 값에 따라 HIGH 또는 LOW 응답합니다.
  s1 += "\n";
  s2 += (R2val)?"HIGH":"LOW";   //R2val 값에 따라 HIGH 또는 LOW 응답합니다.
  s2 += "\n";
  s3 += (R3val)?"HIGH":"LOW";   //R3val 값에 따라 HIGH 또는 LOW 응답합니다.
  s3 += "\n";
  s4 += (R4val)?"HIGH":"LOW";   //R4val 값에 따라 HIGH 또는 LOW 응답합니다.
  s4 += "\n";

  // 클라이언트로 응답을 보냅니다
  client.print(s1);
  delay(1);
  client.print(s2);
  delay(1);
  client.print(s3);
  delay(1);
  client.print(s4);
  delay(1);
  Serial.println("Client disonnected");  // 응답완료 후에는 클라이언트를 닫습니다.
}
