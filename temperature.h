#ifndef CAR_H
#define CAR_H
int EN12 = 2;
int a1 = 6;
int a2 = 5;
int EN34 = 8;
int a3 = 3;
int a4 = 9;
#define EPS (1000)
const double ratio=(1/(15000.0));
void setup_Car() {
  pinMode(EN12, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(EN34, OUTPUT);
  pinMode(a3, OUTPUT);
  pinMode(a4, OUTPUT);
}

void stopping() {
  digitalWrite(EN12, LOW);
  digitalWrite(a1, HIGH);
  digitalWrite(a2, LOW);
  digitalWrite(EN34, LOW);
  digitalWrite(a3, HIGH);
  digitalWrite(a4, LOW);  
}
void backward(uint8_t velocity){ //0 and 255 for velocity
  digitalWrite(EN12, HIGH);
  analogWrite(a1, velocity);
  digitalWrite(a2, LOW);
  digitalWrite(EN34, HIGH);
  analogWrite(a3, velocity);
  digitalWrite(a4, LOW);  
}

void forward(uint8_t velocity){
  digitalWrite(EN12, HIGH);
  digitalWrite(a1, LOW);
  analogWrite(a2, velocity);
  digitalWrite(EN34, HIGH);
  digitalWrite(a3, LOW);
  analogWrite(a4, velocity);
}

void rotateright(uint8_t velocity) {
  digitalWrite(EN12, HIGH);
  analogWrite(a1, velocity); // right wheel forward
  digitalWrite(a2, LOW);
  digitalWrite(EN34, HIGH);
  digitalWrite(a3, LOW); //left wheel backwards
  analogWrite(a4, velocity);
}

void rotateleft(uint8_t velocity) {
  digitalWrite(EN12, HIGH);
  digitalWrite(a1, LOW); // right wheel backward
  analogWrite(a2, velocity);
  digitalWrite(EN34, HIGH);
  analogWrite(a3, velocity); //left wheel forward
  digitalWrite(a4, LOW);
}

void turn_certain_degree(unsigned short direction, unsigned int degree) {
  Serial.print("direction:");Serial.println(direction);
  Serial.print("degree: "); Serial.println(degree);
  Serial.print("ratio:"); Serial.println(ratio);
  double theta = 0.0; 
  if (direction==1)
    rotateleft(100);
  else 
  if (direction==2)
    rotateright(100);
  unsigned int looptime = millis();
  int wx;
  while(1){
    looptime = millis();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68,2,true);
    
    wx = (Wire.read()<<8)|Wire.read();
    Wire.endTransmission(true);
    theta += abs(wx) < EPS ? 0: ratio*abs(wx);
    Serial.println(theta);
    if (theta >= degree) {
      break;
    }
    //while(millis()-looptime < 10);
  }
  Serial.println("turning end");
  stopping();
}

#endif

#ifndef HTTPLIB_H
#define HTTPLIB_H
WiFiClient client;
bool HAS_RESULT;
char* server = "guarded-coast-40036.herokuapp.com";

void postRequest(const char* host, const char* loc, String data ){
    client.stop();
  //body
  if (client.connect(server, 80)) {
    Serial.println("Posting....");
    //Serial.print("post body:"); Serial.println(data);
    client.print("POST ");client.print(loc);client.println(" HTTP/1.1");
    client.print("Host: ");
    client.println(server);
    client.println("Connection: close");
    client.println("Content-Type: application/x-www-form-urlencoded;");
    client.println("User-Agent: Arduino/1.0");
    client.print("Content-Length: ");client.println(data.length());
    //body
    client.println();
    Serial.print("post body:"); Serial.println(data);
    for (int i = 0 ; i < data.length() ; i++)
      client.write(byte(data[i]));
    Serial.println("Post End!");
  }
  else{
    Serial.println("CONNECTION FAILED");
  }
}



void readResponse(){
  // close any connection before send a new request.
  // This will free the socket on the WiFi shield
  //  Serial.println("In read");
  while (client.available()) {
    char c = client.read();
    Serial.print(c);
    HAS_RESULT = true;
  }
}

#endif

#include "myUtils.h"


void setup() {

    Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  
  //setup_main();
  setup_test();
  Serial.println("SETUP END");
  return;
}
void loop() {
  // if ten seconds have passed since your last connection,
  // then connect again and send data:
  //UdpMode();
  //httpMode();
  turn_certain_degree(1,90);
  delay(5000);
}


void UdpMode(){
  receiveUDP();
}


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
    rotateleft(255);
  }
  else
  if (command == 2 ){
    rotateright(255);
  }
  else
  if (command == 3){
    stopping();
  }
  else
  if (command == 4){
    forward(255);
  }
  else
  if (command == 5){
    backward(255);
  }
  else 
  if (command == 6 ){
    temperature_handler();
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
  const int packetBufferLen = 255;
  char packetBuffer[packetBufferLen] = "";
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

