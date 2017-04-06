//--------------------------------------------------------------
//servoWrite, getDistance
//function knockAway {p=180; while(p>=130){turn(p); snooze(15); p--;}}
//function knockToward {turn(180);}
//function knock {i=0; while (i<3) {knockAway; knockToward; snooze(100); i++;}}
//function startup {d=getDist; if ((d>20) && (d<50)) knock; snooze(5000);}
//turn, getDistance

#include "bitlash.h"
#include <Servo.h>

//set servp pin
#define SERVO_PIN 9
//set distance sensor pins
#define TRIG_PIN 2 
#define ECHO_PIN 4 

Servo myServo;

//-------------------------------------------------------------------
numvar servoWrite(void) {
  myServo.write(getarg(1));
}

//-------------------------------------------------------------------
numvar getDistance(void) {
  long distance;
  
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
void setup() 
{  
  initBitlash(57600);
  myServo.attach(SERVO_PIN);
  myServo.write(180);

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  //randomSeed(analogRead(A0));

  addBitlashFunction("getDistance", (bitlash_function) getDistance);
  addBitlashFunction("servoWrite", (bitlash_function) servoWrite);
}

//-------------------------------------------------------------------
void loop() {
  runBitlash();
}


