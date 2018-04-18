#include <Smartcar.h>
Odometer encoder;
Gyroscope gyro;
Car car;
SR04 sideUltra;
SR04 frontUltra;
const int SIDE_TRIGGER = 6;
const int SIDE_ECHO = 5;
const int encoderPin = 2;
const int FRONT_TRIGGER = 6;
const int FRONT_ECHO = 5;
const int INFRARED_PIN = 15;
GP2D120 ir;
void setup() {
  Serial.begin(9600);
  gyro.attach();
  ir.attach(INFRARED_PIN);
  encoder.attach(encoderPin);
  sideUltra.attach(SIDE_TRIGGER, SIDE_ECHO);
  frontUltra.attach(FRONT_TRIGGER, FRONT_ECHO);
  gyro.begin();
  car.begin(encoder, gyro);
}
void loop() {
 
 if (findPlace()) {
    car.setSpeed(0);
    delay(800);
    makeParkR2();
  //  alignPark();
    while(true){}
  }
  else {
    car.setSpeed(30);
    car.setAngle(-10);
  }
}
//Tries to find a parking space of size 35cm, returns true
boolean findPlace(){
if(sideUltra.getDistance() == 0 || sideUltra.getDistance() > 30 ){
  encoder.begin();
  car.setSpeed(30);
  while(sideUltra.getDistance() == 0 || sideUltra.getDistance() > 30){
    if(encoder.getDistance() > 35) {
      car.setSpeed(0);
      car.setAngle(0);
      encoder.begin();
      while(encoder.getDistance () < 12) {
        car.setSpeed(30);
      }
      car.setSpeed(0);
      delay(500);
      return true;
    //  makeParkRotate();
      }
    }
  }
  else {
    return false;
   }
  }
//Method for backing up into the parking space once it's found it, using rotate
void makeParkR(){
 //car.setSpeed(-20);
 gyro.update();
  while(gyro.getAngularDisplacement() > 345 || gyro.getAngularDisplacement() == 0) {
    car.rotate(-5);
    delay(500);
  }
 car.setSpeed(0);
 delay(500);
  gyro.update();
  while(ir.getDistance() > 6 || ir.getDistance() == 0) {
      Serial.println( ir.getDistance());
    car.setSpeed(-30);
  }
  car.setSpeed(0);
  delay(1000);
  //car.setSpeed(30);
  gyro.update();
  car.rotate(15);
  car.setSpeed(0);
}
//Method for backing up into the parking space once it's found it, using setAngle
void makeParkR2(){
 gyro.update();
  while((gyro.getAngularDisplacement() > 340 || gyro.getAngularDisplacement() == 0) && (ir.getDistance() > 6 || ir.getDistance() == 0)) {
    car.setAngle(-15);
    car.setSpeed(-30);
  }
 car.setSpeed(0);
  while(ir.getDistance() > 6) {
    car.setAngle(13);
    car.setSpeed(-30);
  }
  car.setSpeed(0);
  while(checkFrontMin(6) && gyro.getAngularDisplacement() != 0) {
    car.setAngle(15);
    car.setSpeed(30);
  }
}
boolean checkFrontMin(int dist) {

  return frontUltra.getDistance() >= dist || frontUltra.getDistance() == 0;
}
boolean checkBackMin(int dist) {

  return ir.getDistance() >= dist;
}
boolean acceptableAngle() {

  if(gyro.getAngularDisplacement() > 355 || gyro.getAngularDisplacement() < 5) {
    return true;
  }
  else {
    return false;
  }
}
boolean isAligned() {

  if(ir.getDistance() == 5 && frontUltra.getDistance() >= 5 && acceptableAngle()) {
    return true;
  }
  else {
    return false;
  }
}

void alignPark() {
    
  }

