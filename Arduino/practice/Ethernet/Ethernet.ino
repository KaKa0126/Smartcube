#include <SPI.h>
#include <Ethernet.h>
 
byte mac[] = {0x90, 0xA2, 0xDA, 0x10, 0x53, 0x07};
// MAC Address(같은 내부IP에서 겹치지만 않으면 됨) 
IPAddress ip(192, 168, 50, 74);
// IP Address(내부 IP)
// LAN에서 접속 가능. http://192.168.50.74/
EthernetServer server(80); // Port 80 is default for HTTP
 
void setup()
{
    // You can use Ethernet.init(pin) to configure the CS pin
    //Ethernet.init(10);  // Most Arduino shields
    //Ethernet.init(5);   // MKR ETH shield
    //Ethernet.init(0);   // Teensy 2.0
    //Ethernet.init(20);  // Teensy++ 2.0
    //Ethernet.init(15);  // ESP8266 with Adafruit Featherwing Ethernet
    //Ethernet.init(33);  // ESP32 with Adafruit Featherwing Ethernet
    Ethernet.begin(mac, ip);  // initialize Ethernet device
    server.begin(); // Start to listen for clients
    Serial.begin(115200);
}
 
void loop()
{
    EthernetClient client = server.available();  // try to get client
 
    if (client) {  // got client?
        boolean currentLineIsBlank = true;
        while (client.connected()) {
            if (client.available()) {   // client data available to read
                char c = client.read(); // read 1 byte (character) from client
                Serial.print(c);
                // last line of client request is blank and ends with \n
                // respond to client only after last line received
                if (c == '\n' && currentLineIsBlank) {
                    // send a standard http response header
                    client.println("HTTP/1.1 200 OK");
                    client.println("Content-Type: text/html");
                    client.println("Connection: close");
                    client.println();
                    // send web page
                    client.println("<!DOCTYPE html>");
                    client.println("<html lang=\"en\">");
                    client.println("<head>");
                    client.println("<meta charset=\"UTF-8\">");
                    client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
                    client.println("<title>Arduino Web Page</title>");
                    client.println("</head>");
                    client.println("<body>");
                    client.println("<h1>Arduino Web Page</h1>");
                    client.println("<p>Hello World! This is arduino Web page!</p>");
                    client.println("</body>");
                    client.println("</html>");
                    break;
                }
                if (c == '\n') {
                    currentLineIsBlank = true;
                } 
                else if (c != '\r') {
                    currentLineIsBlank = false;
                }
            } 
        } 
        delay(1);  
        client.stop(); // close the connection
    }
}
