#ifndef MYUTILS_H
#define MYUTILS_H
#include <WiFi.h>
#include <WiFiUdp.h>
#include "httplib.h"
#include "temperature.h"
#include "Car.h"
void printCurrentNet();
void printWifiStatus();
void printWifiData();
void httpRequest();
void temperature_handler();
short command = -1;
WiFiUDP Udp;
const int localPort = 2390;



void execute(){
  if (command == 1){
    rotateleft();
  }
  else
  if (command == 2 ){
    rotateright();
  }
  else
  if (command == 3){
    stopping();
  }
  else
  if (command == 4){
    forward();
  }
  else
  if (command == 5){
    backward();
  }
  else 
  if (command == 6 ){
    temperature_handler();
  }      
  else
  if (command == 7){
    accelerate();
  }
  else
  if (command == 8){
    decelerate();
  }
}
void endless_readResponse(){
  HAS_RESULT = false;
  while(!HAS_RESULT){
    readResponse();
  }
}
unsigned int get_degree(char*, unsigned short);
unsigned short get_command(char*, unsigned short);
void execute(char*& buffer, unsigned short& len){
  
  if(buffer[0]=='d'){
    unsigned int degrees = get_degree(buffer,len); 
    unsigned short command = get_command(buffer,len);
    turn_certain_degree(command,degrees);
  }
}


unsigned int get_degree(char* buffer, unsigned short len){
  unsigned int degree = 0 ;
  unsigned short i = 2;
  while( (buffer[i] - '0') <= 9 && (buffer[i] - '0') >= 0 ){
    degree = degree * 10 + (buffer[i] - '0');
    i++;
  } 
  return degree;
}

unsigned short get_command(char* buffer,unsigned short len){
  unsigned short space_pos = 0 ; 
  while( space_pos < len && buffer[space_pos]!='/')space_pos++;
  return buffer[space_pos+1] - '0';
}

void postIp(){
  char* loc = "/ip_addresses";
  IPAddress myIp = WiFi.localIP();
  char ip[20];
  sprintf(ip, "%d.%d.%d.%d", myIp[0], myIp[1], myIp[2], myIp[3]);
  String data = "utf8=%E2%9C%93&ip_address%5Buser%5D=arduino&commit=Create+Ip+address&ip_address%5Bip%5D="+String((char*)ip);
  //post request
  postRequest( server , loc , data);
}

void postTemperature(int& temperature){
  char* loc = "/temperature";
  String data("utf8=%E2%9C%93&author%5B%5D=Arduino2&commit=submit&temperature=");
  data += String( temperature );
  postRequest( server , loc , data);
}

void temperature_handler(){
  read_MPU_data();
  int temperature = Tmp/340.00+36.53;
  postTemperature(temperature);
  endless_readResponse();
}

void setupUDP(){
  Serial.println("\nStarting connection to server...");
  // if you get a connection, report back via serial:
  Udp.begin(localPort);
}
void get_UDP_command(char* buffer, unsigned short len){
  if(buffer[0] == 'd'){
    execute(buffer, len);
  }
  else{
    command = buffer[0] - '0';
    Serial.print("Command: "); Serial.println(command);
    execute();
  }
  
}
void receiveUDP(){
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    const int packetBufferLen = 255;
    char packetBuffer[packetBufferLen] = "";

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
    get_UDP_command(packetBuffer,len);
  }
}

void connectWifi(char* ssid){
  //Serial.println("Connection begin");
  int status = WL_IDLE_STATUS;     // the Wifi radio's status
 // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    status = WiFi.begin(ssid);
    delay(10000);
  }
  //Serial.println("Connection End");

}
void connectResNet(){
  char ssid[] = "ResNet-Open";
  connectWifi(ssid);
}
void connectUCSB(){
  char ssid[] = "UCSB Wireless Web";
  connectWifi(ssid);
}

void connectHotSpot(){
  Serial.println("Connection begin");
  char ssid[] = "CM hotspot";     // the name of your network
  char password[] = "135792468";
  int status = WL_IDLE_STATUS;     // the Wifi radio's status

 // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.println("endless");
    status = WiFi.begin(ssid,password);
    delay(10000);
  }
  Serial.println("Connection End");
}

void uploadIp(){
  postIp();
  HAS_RESULT= false;
  endless_readResponse();
  client.stop();
}
void setup_main(){
  setup_Car();

  Serial.println("SettingUp...");
  setup_temperature();
  /////////////////////////////////////////
  ////chose connection type!
  connectUCSB();
  //connectHotSpot();
  //connectResNet();
  //////////////////////////////////

   IPAddress myIp = WiFi.localIP();
   Serial.println(myIp);
   // important ! setupUDP before uploadIp!!!! or Some strange bug will occur(you can try)
   setupUDP();
   uploadIp();
}

void setup_test(){
  setup_Car();
  Serial.println("SettingUp...");
  setup_temperature();

}
#endif
