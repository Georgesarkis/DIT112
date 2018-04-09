#include <Smartcar.h>
Odometer encoder;
Gyroscope gyro;
Car car;
SR04 ultrasonicSensor;
SR04 ultrasonicSensor2;
GP2Y0A21 sideFrontIR;
const int TRIGGER_PIN = 4;
const int ECHO_PIN = 7;
const int encoderPin = 2;
const int TRIGGER_PIN2 = 6;
const int ECHO_PIN2 = 5;
void setup() {
  Serial.begin(9600);
  gyro.attach();
  encoder.attach(encoderPin);
  ultrasonicSensor.attach(TRIGGER_PIN, ECHO_PIN);
  ultrasonicSensor2.attach(TRIGGER_PIN2, ECHO_PIN2);
  gyro.begin();
  car.begin(encoder, gyro);
}
void loop() {
findPlace();
}
void findPlace(){
if(ultrasonicSensor.getDistance() == 0 || ultrasonicSensor.getDistance() > 20 ){
  encoder.begin();
  car.setSpeed(30);
  while(ultrasonicSensor.getDistance() == 0 || ultrasonicSensor.getDistance() > 20){
    if(encoder.getDistance() > 38) {
      car.setSpeed(0);
      car.setAngle(0);
      makeParkRotate();
            while(true){
           car.setSpeed(0);
              }
      }
    }
  }
  else {
    car.setSpeed(30);
    car.setAngle(-10);
   }
  }
void makeParkRotate(){
   car.setSpeed(-15);
      gyro.update();
      car.rotate(-9);
      car.setSpeed(0);
      delay(2000);
      gyro.update();
      while(ultrasonicSensor2.getDistance() == 0  || ultrasonicSensor2.getDistance() != 12) {
        car.setSpeed(-20);
        }
        car.setSpeed(0);
        delay(500);
     // car.go(-20);
    //  delay(200);
      makeParkL();
  }
void makeParkL(){
  car.setSpeed(30);
  delay(200);
   gyro.update();
      car.rotate(13);
}
void makeItBetter(){
 // int fix = gyro.getAngularDisplacement();
  }
