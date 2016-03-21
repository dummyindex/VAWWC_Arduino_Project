byte server[] = { 173,203,98,29 }; 
HTTPClient client("api.pachube.com",server);
http_client_parameter parameters[] = {
  { "key","afad32216dd2aa83c768ce51eef041d69a90a6737b2187dada3bb301e4c48841" }
  ,{ NULL,NULL }
};

client.postURI("login.wireless.ucsb.edu");

