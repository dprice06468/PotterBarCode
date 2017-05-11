//if user within sensor range
//make frog hop randomly

//turnquick, getdist
//function startup {run distfroghop;};
//function froghop {turnquick(1200); delay(200); turnquick(1500); snooze(5000);};
//function toggle13 {d13=!d13;};
//function distfroghop {d=getdist; print "testing"; if ((d>20) && (d<50)) print "in range"; print "hopping"; froghop;};

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
numvar turnquick(void) {
  myServo.writeMicroseconds(getarg(1));
  return getarg(1);
}

//-------------------------------------------------------------------
void setup() 
{  
  // initialize bitlash and set primary serial port baud
  // print startup banner and run the startup macro
  initBitlash(57600);

  // initialize ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // initialize servo
  myServo.attach(SERVO_PIN);
  myServo.writeMicroseconds(1500);

  // add custom function to handle ping
  addBitlashFunction("ping", (bitlash_function) ping);
  // add custom function to control servo
  addBitlashFunction("turnquick", (bitlash_function) turnquick);
  // add custom function to control ultrasonic sensor
  addBitlashFunction("getdist", (bitlash_function) getdist);
}

//-------------------------------------------------------------------
void loop() {
  //make frog hop
  runBitlash();
}
