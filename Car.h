#ifndef CAR_H
#define CAR_H
int EN12 = 2;
int a1 = 6;
int a2 = 5;
int EN34 = 8;
int a3 = 3;
int a4 = 9;
#define EPS (1000)
const double ratio=(1/(15000.0)*1900/360);
unsigned int velocity = 150;//0 and 255 for velocity

void (*last_command) (void) = NULL ;


void setup_Car() {
  last_command = setup_Car;
  pinMode(EN12, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  pinMode(EN34, OUTPUT);
  pinMode(a3, OUTPUT);
  pinMode(a4, OUTPUT);
}

void stopping() {
  
  last_command = NULL;
  digitalWrite(EN12, LOW);
  digitalWrite(a1, HIGH);
  digitalWrite(a2, LOW);
  digitalWrite(EN34, LOW);
  digitalWrite(a3, HIGH);
  digitalWrite(a4, LOW);  
}
void backward(){ 
  last_command = backward;
  digitalWrite(EN12, HIGH);
  analogWrite(a1, velocity);
  digitalWrite(a2, LOW);
  digitalWrite(EN34, HIGH);
  analogWrite(a3, velocity);
  digitalWrite(a4, LOW);  
}

void forward( ){
  last_command = forward;
  digitalWrite(EN12, HIGH);
  digitalWrite(a1, LOW);
  analogWrite(a2, velocity);
  digitalWrite(EN34, HIGH);
  digitalWrite(a3, LOW);
  analogWrite(a4, velocity);
}

void rotateright( ) {
  last_command = rotateright;
  digitalWrite(EN12, HIGH);
  analogWrite(a1, velocity); // right wheel forward
  digitalWrite(a2, LOW);
  digitalWrite(EN34, HIGH);
  digitalWrite(a3, LOW); //left wheel backwards
  analogWrite(a4, velocity);
}

void rotateleft( ) {
  last_command = rotateleft;
  digitalWrite(EN12, HIGH);
  digitalWrite(a1, LOW); // right wheel backward
  analogWrite(a2, velocity);
  digitalWrite(EN34, HIGH);
  analogWrite(a3, velocity); //left wheel forward
  digitalWrite(a4, LOW);
}

void accelerate(){
  velocity = (velocity + 30)% 255;
  if (last_command == NULL){
    last_command = forward;
  }
  last_command();
}

void decelerate(){
  velocity = (velocity - 30) > 0 ? (velocity - 30) : 0;
  if (last_command == NULL){
    last_command = backward;
  }
  last_command();
}


void turn_certain_degree(unsigned short direction, unsigned int degree) {
  velocity = 100; //set velocity, or maybe not precise enough.
  //Serial.print("direction:");Serial.println(direction);
  //Serial.print("degree: "); Serial.println(degree);
  //Serial.print("ratio:"); Serial.println(ratio);
  double theta = 0.0; 
  
  void (*rotate) (void);  
  if (direction==1)
    rotate = rotateleft;
  else 
  if (direction==2)
    rotate = rotateright;
  else
    return;
  rotate();
  //unsigned int looptime = millis();
  int wx;
  long lasttime=millis();
  long time;
  while(1){
    delay(5);
    setup_temperature();
    //looptime = millis();
    Wire.beginTransmission(MPU_addr);
    Wire.write(0x43);
    Wire.endTransmission(false);
    Wire.requestFrom(0x68,6,true);
    
    double wx = (Wire.read()<<8)|Wire.read();
    double wy = (Wire.read()<<8)|Wire.read();
    double wz = (Wire.read()<<8)|Wire.read();
    time=millis();
    double w = wx*wx + wy*wy + wz*wz ;
    w = sqrt(w); // in deg/sec
    Wire.endTransmission(true);
    //theta += w < EPS ? 0: ratio*w;
    theta += w < EPS ? 0: (float)(time-lasttime)*w/1000.0/131;
    velocity = degree-theta <= 25 ? 50 : velocity;
    rotate();
    lasttime=time;
    //Serial.print("w="); Serial.println(w);
    //Serial.print("theta=");Serial.println(theta);
    if (theta >= degree) {
      break;
    }
    //while(millis()-looptime < 10);
  }
  //Serial.println("turning end");
  velocity = 100;
  stopping();
  
}

#endif

