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
  Serial.print("direction:");Serial.println(direction);
  Serial.print("degree: "); Serial.println(degree);
  Serial.print("ratio:"); Serial.println(ratio);
  double theta = 0.0; 
  if (direction==1)
    rotateleft();
  else 
  if (direction==2)
    rotateright();
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

