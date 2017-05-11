//turn, getdist
//function knockaway {p=180; while(p>=130){turn(p); snooze(15); p--;}}
//function knocktoward {turn(180);}
//function knock {i=0; while (i<3) {knockaway; knocktoward; snooze(100); i++;}}
//function knock {i=0; while (i<3) {knockaway; delay(15); knocktoward; snooze(100); i++;}}
//function distknock {d=getDist; if ((d>20) && (d<50)) knock; snooze(5000);}
//function startup {run distknock;}
//turn, getDistance

#include "bitlash.h"
#include <Servo.h>

//set servo pin
#define SERVO_PIN 9
//set distance sensor pins
#define TRIG_PIN 2 
#define ECHO_PIN 4 

Servo myServo;

//-------------------------------------------------------------------
numvar ping(void) {
  return TCNT1; // return the value of Timer 1
}

//-------------------------------------------------------------------
numvar getdist(void) {
  long distance, duration;
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = (duration/2) / 29.1;
  
  return distance;
}

//-------------------------------------------------------------------
numvar turn(void) {
  myServo.write(getarg(1));
  return getarg(1);
}

//-------------------------------------------------------------------
void setup(void) {

  // initialize bitlash and set primary serial port baud
  // print startup banner and run the startup macro
  initBitlash(57600);

  //initialize ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // initialize the servo
  myServo.attach(SERVO_PIN);
  myServo.write(180);
  
  // add custom function to handle ping
  addBitlashFunction("ping", (bitlash_function) ping);
  // add custom function to control servo
  addBitlashFunction("turn", (bitlash_function) turn);
  // add custom function to control ultrasonic sensor
  addBitlashFunction("getdist", (bitlash_function) getdist);
}

//-------------------------------------------------------------------
void loop(void) {
  runBitlash();
}
