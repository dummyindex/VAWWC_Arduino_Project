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


