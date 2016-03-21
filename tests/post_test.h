#include <WiFi.h>
#include <WiFiUdp.h>
WiFiClient client;
bool HAS_RESULT = false;
void postRequest(){
  //https://guarded-coast-40036.herokuapp.com/ip_addresses/
  char server[] = "guarded-coast-40036.herokuapp.com";
  client.stop();
  //body
  if (client.connect(server, 80)) {
    IPAddress myIp = WiFi.localIP();
    Serial.println(myIp);
    char ip[200];
    sprintf(ip, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
    
    String data = "utf8=%E2%9C%93&authenticity_token=gd1W0ZZ6DAPQD5d67A7DfL3bhcTfktLNaWAt7mJmb3Zl3Mo32nJHwaA6M6WKCuYB7sTFLKv34nkQIxh3%2B1R82g%3D%3D&ip_address%5Buser%5D=fromArduino&commit=Create+Ip+address&ip_address%5Bip%5D="+String((char*)ip);
    
    Serial.println("Posting....");
    //Serial.print("post body:"); Serial.println(data);
    client.println("POST /ip_addresses HTTP/1.1");
    client.print("Host: guarded-coast-40036.herokuapp.com");
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");client.println(data.length());
    //body

    client.println();
    client.println(data);
    
  }
  else{
    Serial.println("CONNECTION FAILED");
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

void connectWifi(char* ssid){
  //Serial.println("Connection begin");
  int status = WL_IDLE_STATUS;     // the Wifi radio's status
 // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    status = WiFi.begin(ssid);
    delay(2000);
  }
  //Serial.println("Connection End");
}

void connectHotSpot(){
  Serial.println("Connection begin");
  char ssid[] = "CM hotspot";     // the name of your network
  char password[] = "135792468";
  int status = WL_IDLE_STATUS;     // the Wifi radio's status

 // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    status = WiFi.begin(ssid,password);
    delay(2000);
  }
  Serial.println("Connection End");
}
void test(){
  HAS_RESULT= false;
  postRequest();
  while(!HAS_RESULT)
    readRequest();  
}
