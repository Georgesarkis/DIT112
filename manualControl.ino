#include <Smartcar.h>
SR04 ultrasoniceSensor1;
SR04 ultrasoniceSensor2;

GP2D120 sideFrontIR; //measure distances between 5 and 25 centimeters
const int SIDE_FRONT_PIN = A0; //you can use only analog enabled pins
Odometer encoderLeft, encoderRight;
Gyroscope gyro;
Car car; 
Odometer encoder;  // Odometer

char data = 0;            //Variable for storing received data
const int fSpeed = 70; //70% of the full speed forward
const int bSpeed = -70; //70% of the full speed backward
const int lDegrees = -75; //degrees to turn left
const int rDegrees = 75; //degrees to turn right 
const int encoderPin = 2; //Odometer
const int TRIGGER_PIN1 = 6; // front sensor on pin 6 and 5
const int ENCHO_PIN1 = 5; 
const int TRIGGER_PIN2 = 9;  //side sensor on pin 9 and 8
const int ENCHO_PIN2 = 8;

void setup() {
  Serial.begin(9600);   //Sets the baud for serial data transmission                               
  pinMode(13, OUTPUT);  //Sets digital pin 13 as output pin
  sideFrontIR.attach(SIDE_FRONT_PIN);
  ultrasoniceSensor1.attach(TRIGGER_PIN1,ENCHO_PIN1);
  ultrasoniceSensor1.attach(TRIGGER_PIN2,ENCHO_PIN2);
  encoder.attach(encoderPin);
  encoder.begin();
  gyro.attach();
  encoderLeft.attach(2);
  encoderRight.attach(3);
  encoderLeft.begin();
  encoderRight.begin();
  car.enableCruiseControl();
  gyro.begin();
  car.begin(encoderLeft, encoderRight,gyro); //initialize the car using the encoders and the gyro
}

void loop() {
     if(Serial.available() > 0)      // Send data only when you receive data:
   {
      data = Serial.read();        //Read the incoming data & store into data        
      if(data == 'A'){              // Checks whether value of data is equal to 1
         autoPark();
         }   //If value is 1 then LED turns ON
      else if(data == 'M'){         //  Checks whether value of data is equal to 0
         handleInput();
         }
   }
  
}
void autoPark(){
  while(sideFrontIR.getDistance() == 0 || sideFrontIR.getDistance() < 10){
      car.go(40);
      if(encoder.getDistance() == 50){
        car.setAngle(-75);
        car.go(10);
        car.setAngle(75);
        car.go(-10);
      }
  }
}

void handleInput() { //handle serial input if there is any
  while(sideFrontIR.getDistance() == 0 || sideFrontIR.getDistance() < 10){
    if (Serial3.available()) {
      char input = Serial3.read(); //read everything that has been received so far and log down the last entry
      switch (input) {
        case 'L': //rotate counter-clockwise going forward
          car.setSpeed(fSpeed);
          car.setAngle(lDegrees);
          break;
        case 'R': //turn clock-wise
          car.setSpeed(fSpeed);
          car.setAngle(rDegrees);
          break;
        case 'F': //go ahead
          car.setSpeed(fSpeed);
          car.setAngle(0);
          break;
        case 'B': //go back
          car.setSpeed(bSpeed);
          car.setAngle(0);
          break;
        default: //if you receive something that you don't know, just stop
          car.setSpeed(0);
          car.setAngle(0);
          break;
      }
    }
  }
}
