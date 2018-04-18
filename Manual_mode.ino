#include <Smartcar.h>

Odometer encoder;
Gyroscope gyro;
Car car;
SR04 frontUltra;
SR04 sideUltra;
GP2D120 ir;

// when we combine the code we have to check the pins and initilaize the sensors.
const int FRONT_TRIGGER = 6;
const int FRONT_ECHO = 5;
const int SIDE_TRIGGER = 6;
const int SIDE_ECHO = 5;
const int INFRARED_PIN = 15;

// Attributes that will be changed during the loop. 
char mode;
char action;


void setup() {
  Serial3.begin(9600);
  
  frontUltra.attach(FRONT_TRIGGER,FRONT_ECHO);
  sideUltra.attach(SIDE_TRIGGER, SIDE_ECHO);
  ir.attach(INFRARED_PIN);
  
  gyro.attach();
  encoder.attach(2);
  
  encoder.begin();
  gyro.begin();
  car.begin(encoder ,gyro);
  
  // Add these.
  action = 0; 
  mode = 'm';
}

void loop() {
  if(mode = 'm'){
    while(Serial3.available()){
     action = Serial3.read();
     switch(action){
      
     case 'f' : //Forward 
      if(NoCrashFront){
       car.setSpeed(80);
       car.setAngle(0);
      }
      break;
      
     case 'b' : //Backwards
      if(NoCrashBack){
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

     case 'm' :// mode change
      mode = 'a';
      car.setSpeed(0);
      break;
      
     case 's' :
      car.setSpeed(0);
      break;  

      default: car.setSpeed(0); // Might have to create another thing.
     //manual_mode();
    }
  }
  }
  else {
    // Put in automatic mode and parking here.
    }
  }
  
  boolean NoCrashFront(){
  if(frontUltra.getDistance() > 0 && frontUltra.getDistance()> 10){
    return false;
    car.setSpeed(0);
  }
  return true;
}

boolean NoCrashBack(){
  if(ir.getDistance() > 0 && ir.getDistance()> 10){
    return false;
    car.setSpeed(0);
  }
  return true;
}

