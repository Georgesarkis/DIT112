/*
* Display5vOrless sketch
* prints the voltage on analog pin to the serial port
* Warning - do not connect more than 5 volts directly to an Arduino pin.
*/

const float referenceVolts = 5.0; // the default reference on a 5-volt board
const int batteryPin = 0;         // battery is connected to analog pin 0

void setup() 
{ 
Serial.begin(9600);
Serial2.begin(9600);
}

void loop()
{ int volts= 5;
  int val = analogRead(batteryPin); // read the value from the sensor 
  if(val/1023.0<volts){
      volts = (val / 1023.0) * referenceVolts; // calculate the ratio
      Serial.print("Volts: ");
      Serial.println(volts); // print the value in volts
  }
  delay(500);
  Serial2.write(volts);
  }
