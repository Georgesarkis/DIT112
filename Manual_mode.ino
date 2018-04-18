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
    action = Serial3.read();
    
    switch(action){
      
    case 'f' : //Forward 
      moveDstyle(3);
      
    case 'b' : //Backwards
      moveBackwardsDstyle(-3);

    case 'r' : // rotate right
      rotateDstyle(5);
      
    case 'l' : // rotate left
      rotateDstyle(-5);

    case 'm' :// mode change
      mode = 'a';
      car.setSpeed(0);
      //car.stop();
      
    case 's' :
      car.setSpeed(0);
      //car.stop();  

    //default: car.stop(); // Might have to create another thing.
     //manual_mode();
    }
  }
  else {
    // Put in automatic mode and parking here.
  }
}

  void moveDstyle(int dir){
    if(NoCrashDstyle() && dir > 0){
    car.go(80);
    }
  }

  void moveBackwardsDstyle(int dir){
    car.setSpeed(-80);
  }

  void rotateDstyle(int dir){
    car.setAngle(dir);
  }
  
  boolean NoCrashDstyle(){
  if(frontUltra.getDistance() > 0 && frontUltra.getDistance()> 6){
    return false;
    car.setSpeed(0);
  }
  return true;
}

boolean NoCrashDoggyStyle(){
  if(ir.getDistance() > 0 && ir.getDistance()> 6){
    return false;
    car.setSpeed(0);
  }
  return true;
}

