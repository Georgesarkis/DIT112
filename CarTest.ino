#include <Smartcar.h>

/*
 Initialize Sensors include :
 Odometer; sensor that measures traveled distance
 Gyroscope; sensor that measures angular velocity in X,Y,Z axis
 SR04; sensors that measure distance
 GP2D120; sensors that measure distance (with angles)
*/

Odometer encoder;
Gyroscope gyro;
Car car;
SR04 sideUltra;
SR04 frontUltra;
GP2D120 ir;

//inputs that arduino takes from bluetooth
char action;
//the car destination from the object in side front and back"ir" sensorse
int sideDist, frontDist, irDist;
//inputs that is going to read from raspberry  (g OR r)
char rOrG;
// To detect color from Pi ... True --> Green   False --> Red
boolean color;



// Pins on Arduino board:

const int SIDE_TRIGGER = 6;
const int SIDE_ECHO = 5;
const int FRONT_TRIGGER = 30 ;
const int FRONT_ECHO = 31 ;
const int encoderPin = 2;
const int INFRARED_PIN = 15;
int speed = 30;



void setup() {

  //bluetooth serial
  Serial2.begin(9600);
  //pi serial
  Serial.begin(9600);
  //Initialize color detect as True (Green) at start
  color = true;
  //input from the bluetooth
  action = 0;

  // Attach Sensors to pins
  gyro.attach();
  ir.attach(INFRARED_PIN);
  encoder.attach(encoderPin);
  sideUltra.attach(SIDE_TRIGGER, SIDE_ECHO);
  frontUltra.attach(FRONT_TRIGGER, FRONT_ECHO);
  // Start Car attached to encoder and gyro + Start gyro
  gyro.begin();
  car.begin(encoder, gyro);

}


void loop() {

  // Update Senors in each loop
  sideDist = sideUltra.getDistance();
  frontDist = frontUltra.getDistance();
  irDist = ir.getDistance();


  /*
     Check what Pi Serial will send and store it
     If Pi sends Green ---> color == True
     If Pi send Red ---> color = False
  */

  if(Serial.available()) {
    rOrG = Serial.read();
  }

  if(rOrG == 'g') {
    color = true;
  }
  else if(rOrG == 'r') {
    color = false;
  }


  //Reads from bluetooth only when the color is true which mean raspberry pi see a green object

  while(Serial2.available() && color == true){

   // Read from bluetooth Serial
     action = Serial2.read();

 switch(action){

  //Forward and check if there is an object in front of it in less then 15 cm
     case 'f' :

      if(frontDist == 0 || frontDist > 15){
       car.setSpeed(speed);
       car.setAngle(0);
      }
      else {
        car.setSpeed(0);
      }
      break;

  //Backwards and check if there is an object in back of it in less then 15 cm
     case 'b' :

      if(irDist == 0 || irDist > 15){
       car.setSpeed(-speed);
       car.setAngle(0);
        }
        else {
          car.setSpeed(0);
        }
      break;

 // rotate right
     case 'r' :

      car.setSpeed(60);
      car.setAngle(80);
      break;
 // rotate left
     case 'l' :

      car.setSpeed(60);
      car.setAngle(-80);
      break;

  // starts the parking mode
     case 'p':

        while(!Serial2.available()){

        if (findPlace()) { // <-- This method will return True if place is found ,Flase if not !
          car.setSpeed(0); // |----   Stop the car after finding place
          delay(800);     //  |----   Wait 800 Ms before start parking
          makeParkRotate();  // <-- This method Will park the car after place is found
          break;
        }

        else {   // <-- If the car did not find a place to park it will continue forwoard !
          car.setSpeed(30);
          car.setAngle(0);
        }
       }
	  case '1': //set speed to 25
	  speed = 15;
	  break;

	  case '2': //set speed to 50
	  speed = 30;
	  break;

	  case '3': //set speed to 75
	  speed = 45;
	  break;

	  case '4': //set speed to 100
	  speed = 60;
	  break;

  //default mode where car will stop whatever input it gets that is not a case from the bluetooth
     default: car.setSpeed(0);

      }  // <-- End of switch
} // <-- End of While


 // Check for obstacles while going backwards
    if(car.getSpeed()<0){
    if(irDist != 0 && irDist < 15){
      car.setSpeed(0);
      car.setAngle(0);
    }
  }
 // Check for obstacles while going forward
  else if(car.getSpeed() > 0){
    if(frontDist != 0 && frontDist < 15){
      car.setSpeed(0);
      car.setAngle(0);
       }
    }



}  // <-- End of loop



//Tries to find a parking space of size 35cm, and returns True

boolean findPlace(){

int sideDist = sideUltra.getDistance(); // <-- Update side Sensors

if(sideDist == 0 || sideDist > 30 ){   // <-- If side sensor detect space with 30 cm (wide)
    encoder.begin();  // <-- Travel distance will start from 0 after detect space
    car.setSpeed(30);  // <-- Make car speed slow 30

    while(sideDist == 0 || sideDist > 30){ // <-- While the sensor detecting space 30 cm
    sideDist = sideUltra.getDistance();  // <-- Update the side sens

    if(encoder.getDistance() > 35) { // <-- If traveled distance from the start of detecting  == 35 which mean fit the car
      car.setSpeed(0);
      car.setSpeed(0);
      delay(500);
      return true;   // <-- Stop the car and return True
      }
      }
  }

  else { // <-- For the first if.
    return false; // <-- If there is no space will return False
   }

  }


//Method for parking after finding suitable space
void makeParkRotate(){

 int gy = gyro.getAngularDisplacement(); // <-- Initialize gy variable to store car angle
 gyro.update(); // <-- Update Gyroscope to to get the latest measurements from the gyroscope

/* Rotate 20 Degree opposite clockwise after the car find place to park and stop */
  while(gy > 340 || gy == 0) { // <-- While the car is not 20 Degree opposite clockwise
    gy = gyro.getAngularDisplacement(); // <-- Update car angle
    car.rotate(-7); // <-- Rotate the car 5 Degree opposite clockwise
    car.setSpeed(0); // <-- Stop the car a bit
    delay(400); // <-- Wait 400 ms
  }

// Stop the car a bit and wait 500 ms
 car.setSpeed(0);
 delay(500);

/* Going backward after rotating until 5 cm with Infrared Sensor */
  int irDist = ir.getDistance();
  while(irDist > 5 ) {  // <-- While the car is not 5 cm far from the back of the it
    car.setSpeed(-25);  // <-- Go backward with speed 25
    irDist = ir.getDistance();  // <-- Update Infrared Sensor
  }

// Stop the car a bit and wait 500 ms
  car.setSpeed(0);
  delay(500);

/*
   Rotate nearly 25 Degree clockwise to normal position
   after the first rotation and going backward
*/
  while( gy > 7 ) {  // <-- While the car is not nearly 5 -- 7 Degree clockwise

    gy = gyro.getAngularDisplacement();  // <-- Update car angle
    car.rotate(4);  // <-- Rotate the car 4 Degree clockwise
    car.setSpeed(0); // <-- Stop the car a bit
    delay(400); // <-- Wait 400 ms
  }

/* Fix the car after parking */
  car.rotate(-5); // <-- Rotate the car 5 Degree opposite clockwise
  car.go(-5); // <-- Going backward 5 cm
  car.setSpeed(0); // <-- Stop the car

}
