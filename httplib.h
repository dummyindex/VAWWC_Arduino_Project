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

