//ping, pushup, dropdown
//function upanddown {pushup;snooze(2000);dropdown;snooze(5000);}
//function updntrig {if(d4==1)upDn;}
//function startup {run upanddown,5000;}

#include "bitlash.h"
#include <Servo.h>

//set servo pin
#define SERVO_PIN 9
//#define TRIGGER_PIN 4
#define TOP 50
#define BOTTOM 120

Servo myServo;

//-------------------------------------------------------------------
numvar ping(void) {
  return TCNT1; // return the value of Timer 1
}

//--------------------------------------------------------------
numvar pushup(void) {
  for (byte pos=BOTTOM; pos>=TOP; pos--)     // goes from 180 degrees to 0 degrees 
  {                                
    myServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}

//--------------------------------------------------------------
numvar dropdown(void) {
    myServo.write(BOTTOM);              // tell servo to go to position in variable 'pos' 
}

//-------------------------------------------------------------------
void setup() {
  // initialize bitlash and set primary serial port baud
  // print startup banner and run the startup macro
  initBitlash(57600);

  myServo.attach(SERVO_PIN);
  myServo.write(BOTTOM);
  
  // add custom function to handle ping
  addBitlashFunction("ping", (bitlash_function) ping);
  // add custom function to push candy stick up
  addBitlashFunction("pushup", (bitlash_function) pushup);
  // add custom function to drop candy stick down
  addBitlashFunction("dropdown", (bitlash_function) dropdown);
}

//-------------------------------------------------------------------
void loop() {
  runBitlash();

  //if (digitalRead(TRIGGER_PIN) == HIGH)
  //{
  //      pushUp();
  //      delay(2000);
  //      dropDown();
  //      delay(5000);
  //}
}
