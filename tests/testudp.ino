#include "udptest.h"
void setup() {
  Serial.begin(9600); 
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println("In setup");
  connectResNet();
  setupUDP();
}

void loop() {
  receiveUDP();
}
