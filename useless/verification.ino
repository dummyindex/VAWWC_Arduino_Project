void setup(){
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("SettingUp...");
  connectUCSB();
  getVerify();
  HAS_RESULT = false;
  while(!HAS_RESULT)
    readRequest();
  HAS_RESULT = false;
  postRequest();
  while(!HAS_RESULT)
    readRequest();
}


void loop(){

}
