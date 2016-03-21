void get_command(){
  if( BUFFER[0] == '$' && BUFFER[1] == '$' && BUFFER[2] == '$'){
    command = BUFFER[3] - '0';
    Serial.print("command:");Serial.println(command);
    execute();
  }
}

void readRequest(){
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  //  Serial.println("In read");
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
    for(int i = 0 ; i < BUFFER_LEN - 1 ; i++)
      BUFFER[i] = BUFFER[i+1];
    BUFFER[BUFFER_LEN - 1] = c;
    get_command(); 
    HAS_RESULT = true;
  }
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
    client.println("privateInfo!Deleted!");
    client.println();
  }
  else{
    Serial.println("CONNECTION FAILED");
  }    
}

void httpRequest() {
  char server[] = "guarded-coast-40036.herokuapp.com";
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

void getVerify() {
  //https://login2.wireless.ucsb.edu/?switch_url=https://login.wireless.ucsb.edu/login.html&ap_mac=bc:67:1c:59:13:f0&client_mac=78:c4:0e:03:9b:e8&wlan=UCSB%20Wireless%20Web&redirect=guarded-coast-40036.herokuapp.com/command

  char server[] = "login2.wireless.ucsb.edu";
  client.stop();
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /?switch_url=https://login.wireless.ucsb.edu/login.html&ap_mac=bc:67:1c:59:13:f0&client_mac=78:c4:0e:03:9b:e8&wlan=UCSB%20Wireless%20Web&redirect=guarded-coast-40036.herokuapp.com/command HTTP/1.1");
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
