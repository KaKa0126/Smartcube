#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
IPAddress server(115,91,83,140);  // numeric IP for 6th Floor Development Server (no DNS)
//char server[] = "http://115.91.83.140";    // name address for 6th Floor Development Server (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 50, 141);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

void Connecting(){
  while(!client.connect(server, 80)){
    Serial.println("connection failed. Retry");
    delay(10);
  }
}

void Disconnecting(){
  Serial.println("success!");
  client.stop();
  if (!client.connected()) {
    Serial.println("disconnecting.");
  }
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection:  
  // try to congifure using static IP address instead of DHCP:
  Ethernet.begin(mac, ip);
  Serial.print("static IP address is ");
  Serial.println(Ethernet.localIP());
  
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");
  
  // give the Ethernet shield five second to initialize:
  delay(5000);

  Connecting();
  
  Serial.print("connected to ");
  Serial.println(client.remoteIP());

  client.stop();
}

void loop() {
  char input = Serial.read();

  switch(input){
    case 'A':
      Connecting();
    
      // Make a HTTP request:
      client.println("GET http://115.91.83.140/laundry_alim/laundry_alim.php");

      Disconnecting();      
      break;
  }
}
