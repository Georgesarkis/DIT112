#include <Smartcar.h>
Odometer encoder;
Gyroscope gyro;
Car car;
SR04 sideUltra;
SR04 frontUltra;
const int SIDE_TRIGGER = 6;
const int SIDE_ECHO = 5;
const int encoderPin = 2;
const int FRONT_TRIGGER = 30 ;
const int FRONT_ECHO = 31 ;
const int INFRARED_PIN = 15;
GP2D120 ir;

//inputs that arduino takes from bluetooth
char action;
//the car destination from the object in side front and back"ir" sensorse
int sideDist, frontDist, irDist;
//inputs that is going to read from raspberry
char pi;
//if pi sees red object this will false, and it will not start moving until it sees grean object
boolean color;

void setup() {
  //bluetooth serial
  Serial2.begin(9600);
  //pi serial
  Serial.begin(9600);
  
  gyro.attach();
  ir.attach(INFRARED_PIN);
  encoder.attach(encoderPin);
  sideUltra.attach(SIDE_TRIGGER, SIDE_ECHO);
  frontUltra.attach(FRONT_TRIGGER, FRONT_ECHO);
  gyro.begin();
  car.begin(encoder, gyro);
  //input from the bluetooth
  action = 0;
  //default mode when first time the car is started
  color = true; 
  
}
void loop() {
  sideDist = sideUltra.getDistance();
  frontDist = frontUltra.getDistance();
  irDist = ir.getDistance();
  //reads from the raspberry pi and sets color to false if there is red object and true if there is green object
  
  //reads from bluetooth only when the color is true which mean raspberry pi seen a green object
	while(Serial2.available() && color == true){
     action = Serial2.read();
     switch(action){
      
     case 'f' : //Forward and check if there is an object in front of it in less then 10 cm
      if(frontDist == 0 || frontDist > 15){
       car.setSpeed(35);
       car.setAngle(0);
      }
      else {
        car.setSpeed(0);
      }
      break;
      
     case 'b' : //Backwards and check if there is an object in back of it in less then 10 cm
      if(irDist == 0 || irDist > 15){
       car.setSpeed(-35);
       car.setAngle(0);
        }
        else {
          car.setSpeed(0);
        }
      break;
        
     case 'r' : // rotate right
      car.setSpeed(60);
      car.setAngle(80);
      break;
      
     case 'l' : // rotate left
      car.setSpeed(60);
      car.setAngle(-80);
      break;
 
	 
  	 case 'p': // starts the parking mode
        while(!Serial2.available()){
  			if (findPlace()) { //if car found good place to park the car will start parking
    			car.setSpeed(0);
    			delay(800);
    			makeParkRotate();
    		   // alignPark();
    			break;
  		  }
       
  		  else {          // else the car will go forward
    			car.setSpeed(30);
    			car.setAngle(0);
  		  }
       }
        
     //default mode where car will stop whatever input it gets that is not a case from the bluetooth
     default: car.setSpeed(0); // Might have to create another thing.
      }
  }
    if(car.getSpeed()<0){
    if(irDist != 0 && irDist < 15){
      car.setSpeed(0);
      car.setAngle(0);
    }
  }
  else if(car.getSpeed() > 0){
    if(frontDist != 0 && frontDist < 15){
      car.setSpeed(0);
      car.setAngle(0);
    }
    }
  
  
}

//Tries to find a parking space of size 35cm, returns true
boolean findPlace(){
  int sideDist = sideUltra.getDistance();
if(sideDist == 0 || sideDist > 30 ){
  encoder.begin();
  car.setSpeed(30);
  while(sideDist == 0 || sideDist > 30){
    sideDist = sideUltra.getDistance();
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
  int irDist = ir.getDistance();
  while(irDist > 5 ) {
    car.setSpeed(-25);
    irDist = ir.getDistance();
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
