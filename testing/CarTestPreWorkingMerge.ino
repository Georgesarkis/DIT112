#include <Smartcar.h>
Odometer encoder;
Gyroscope gyro;
Car car;
SR04 sideUltra;
SR04 frontUltra;
const int SIDE_TRIGGER = 6;
const int SIDE_ECHO = 5;
const int encoderPin = 2;
const int FRONT_TRIGGER = 8 ;
const int FRONT_ECHO = 7 ;
const int INFRARED_PIN = 15;
GP2D120 ir;
char action;
int sideDist, frontDist, irDist;
void setup() {
  Serial2.begin(9600);
  //Serial.begin(9600);
  gyro.attach();
  ir.attach(INFRARED_PIN);
  encoder.attach(encoderPin);
  sideUltra.attach(SIDE_TRIGGER, SIDE_ECHO);
  frontUltra.attach(FRONT_TRIGGER, FRONT_ECHO);
  gyro.begin();
  car.begin(encoder, gyro);
  
    // Add these.
  action = 0; 
}
void loop() {
  sideDist = sideUltra.getDistance();
  frontDist = frontUltra.getDistance();
  irDist = ir.getDistance();
    while(Serial2.available()){
     action = Serial2.read();
     switch(action){
      
     case 'f' : //Forward 
      if(frontDist != 0 && frontDist < 10){
       car.setSpeed(80);
       car.setAngle(0);
      }
      break;
      
     case 'b' : //Backwards
      if(irDist != 0 && irDist < 10){
       car.setSpeed(-100);
       car.setAngle(0);
        }
      break;
        
     case 'r' : // rotate right
      car.setSpeed(45);
      car.setAngle(80);
      break;
      
     case 'l' : // rotate left
      car.setSpeed(45);
      car.setAngle(-80);
      break;
 
     
     case 'p':
            if (findPlace()) {
            car.setSpeed(0);
            delay(800);
            makeParkRotate();
          //  alignPark();
            break;
          }
          else {
            car.setSpeed(30);
            car.setAngle(0);
          }
   default: car.setSpeed(0); // Might have to create another thing.
    }
}
}
//Tries to find a parking space of size 35cm, returns true
boolean findPlace(){
  //int sideDist = sideUltra.getDistance();
if(sideDist == 0 || sideDist > 30 ){
  encoder.begin();
  car.setSpeed(30);
  while(sideDist == 0 || sideDist > 30){
    //sideDist = sideUltra.getDistance();
    if(encoder.getDistance() > 35) {
      car.setSpeed(0);
      car.setSpeed(0);
      delay(500);
      return true;
      }
    }
  }
  else {
    return false;
   }
  }
//Method for backing up into the parking space once it's found it, using rotate
void makeParkRotate(){
 int gy = gyro.getAngularDisplacement();
 
 gyro.update();
 
  while(gy > 340 || gy == 0) {
    gy = gyro.getAngularDisplacement();
    car.rotate(-5);
    car.setSpeed(0);
    delay(400);
  }
 car.setSpeed(0);
 delay(500);
  //int irDist = ir.getDistance();
  while(irDist > 5 ) {
    car.setSpeed(-25);
    //irDist = ir.getDistance();
  }
  car.setSpeed(0);
  delay(500);
  while( gy > 7 ) {
    
    gy = gyro.getAngularDisplacement();
    car.rotate(4);
    car.setSpeed(0);
    delay(400);
  }
  car.rotate(-5);
  car.go(-5);
  car.setSpeed(0);
}

