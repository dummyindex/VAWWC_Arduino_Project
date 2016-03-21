#include <WiFi.h>
#include <WiFiUdp.h>
void printCurrentNet();
void printWifiStatus();
void printWifiData();
void httpRequest();


WiFiClient client;
short command = -1;
const short BUFFER_LEN = 5;
char BUFFER[BUFFER_LEN] = "asd";
const int packetBufferLen = 20;
char packetBuffer[packetBufferLen] = "";
bool HAS_RESULT;
WiFiUDP Udp;
const int localPort = 2390;

void connectWifi(char* ssid){
  Serial.println("Connection begin");
  int status = WL_IDLE_STATUS;     // the Wifi radio's status
 // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    status = WiFi.begin(ssid);
    delay(2000);
  }
  Serial.println("Connection End");
}

void connectUCSB(){
  char ssid[] = "UCSB Wireless Web";
  connectWifi(ssid);
}
void postRequest(){
  char server[] = "login.wireless.ucsb.edu";
  WiFiClient client;
  client.stop();
  //body
  if (client.connect(server, 80)) {
    Serial.println("In Verification connecting....");
    client.println("POST /login.html HTTP/1.1");
    client.println("Host: login.wireless.ucsb.edu");
    client.println("Connection: keep-alive");
    client.println("Content-Length: 118");
    client.println("Cache-Control: max-age=0");
    client.println("Accept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8");
    client.println("Origin: https://login2.wireless.ucsb.edu");
    client.println("Upgrade-Insecure-Requests: 1");
    client.println("User-Agent: Mozilla/5.0 (Windows NT 10.0; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/48.0.2564.116 Safari/537.36");
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Referer: https://login2.wireless.ucsb.edu/?switch_url=https://login.wireless.ucsb.edu/login.html&ap_mac=bc:67:1c:59:13:f0&client_mac=78:c4:0e:03:9b:e8&wlan=UCSB%20Wireless%20Web&redirect=guarded-coast-40036.herokuapp.com/command");
    client.println("Accept-Encoding: gzip, deflate");
    client.println("Accept-Language: en-US,en;q=0.8");

    //body
    client.println();
    client.print("");
  }
  else{
    Serial.println("CONNECTION FAILED");
  }    
}

void getVerify() {
  //https://login2.wireless.ucsb.edu/?switch_url=https://login.wireless.ucsb.edu/login.html&ap_mac=bc:67:1c:59:13:f0&client_mac=78:c4:0e:03:9b:e8&wlan=UCSB%20Wireless%20Web&redirect=guarded-coast-40036.herokuapp.com/command

  char server[] = "login2.wireless.ucsb.edu";
  client.stop();
  // if there's a successful connection:
  while(1){
    if (client.connect(server, 80)) {
      Serial.println("connecting...");
      // send the HTTP PUT request:
      client.println("GET /?switch_url=https://login.wireless.ucsb.edu/login.html&ap_mac=bc:67:1c:59:13:f0&client_mac=78:c4:0e:03:9b:e8&wlan=UCSB%20Wireless%20Web&redirect=guarded-coast-40036.herokuapp.com/command HTTP/1.1");
      client.println("Host: login2.wireless.ucsb.edu");
      client.println("User-Agent: ArduinoWiFi/1.1");
      client.println();
      // note the time that the connection was made:
      break;
    } else {
      // if you couldn't make a connection:
      Serial.println("connection failed");
      Serial.println("retrying");
    }
  }
}

void readRequest(){
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  //  Serial.println("In read");
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
    HAS_RESULT = true;
  }
}
