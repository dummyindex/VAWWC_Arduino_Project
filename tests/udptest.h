#ifndef MYUTILS_H
#define MYUTILS_H
#include <WiFi.h>
#include <WiFiUdp.h>
void printCurrentNet();
void printWifiStatus();
void printWifiData();
unsigned long lastConnectionTime = 0;            // last time you connected to the server, in milliseconds
const double postingInterval = 1 * 1000L; // delay between updates, in milliseconds
WiFiClient client;
char server[] = "guarded-coast-40036.herokuapp.com";
short command = -1;
const short BUFFER_LEN = 5;
char BUFFER[BUFFER_LEN] = "asd";
const int packetBufferLen = 255;
char packetBuffer[packetBufferLen] = "";
bool HAS_RESULT;
const int localPort = 2390;
WiFiUDP Udp;

void setupUDP(){
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}

void receiveUDP(){
  Serial.println("In receiveUDP");
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remoteIp = Udp.remoteIP();
    Serial.print(remoteIp);
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    int len = Udp.read(packetBuffer, 255);
    if (len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println("Contents:");
    Serial.println(packetBuffer);
  }
   Serial.println("Receive End");
}
void connectResNet(){
  Serial.println("Connection begin");
  //char ssid[] = "CM hotspot";
  char ssid[] = "ResNet-Open";     // the name of your network
  //char password[] = "135792468";
  int status = WL_IDLE_STATUS;     // the Wifi radio's status

 // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    status = WiFi.begin(ssid);
    delay(2000);
  }
  Serial.println("Connection End");
  printWifiStatus();
  printWifiData();
}
void readRequest(){
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield

  while (client.available()) {
    char c = client.read();
    for(int i = 0 ; i < BUFFER_LEN - 1 ; i++)
      BUFFER[i] = BUFFER[i+1];
    Serial.print(c);
    BUFFER[BUFFER_LEN - 1] = c;
    HAS_RESULT = true;
  }
}

void httpRequest() {
  client.stop();
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /command HTTP/1.1");
    client.println("Host: guarded-coast-40036.herokuapp.com");
    client.println("User-Agent: ArduinoWiFi/1.1");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
  }
}

void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);
  
  // print your MAC address:
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
 
}
#endif
