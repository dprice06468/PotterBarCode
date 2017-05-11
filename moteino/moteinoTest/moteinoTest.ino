//icecream window proximity action
//function icwproxact {if(time2run) {print "setcolor"; setcolor(255,0,0); print "hopping"; froghop;}}
//icecream IR proximity action
//function icwiract {print "setcolor"; setcolor(255,255,0); print "candy"; upanddown;}

//door proximity action
//function doorproxact {if(time2run) {}}
//door IR action
//function dooriract {}

//tattings window proximity action
//function tatwproxact {if(time2run) {print "setcolor"; setcolor(0,255,0); print "audio"; tataudio;}}
//tattings window IR action
//function tatwiract {print "setcolor"; setcolor(0,255,255); print "fan"; runfan}

//audio commands
//tataudio
//frogaudio

//prox commands
//getdist

//frog commands
//turnquick(#)
//function startup {run distfroghop;};
//function froghop {turnquick(1200); delay(200); turnquick(1500);};
//function toggle13 {d13=!d13;};
//function distfroghop {d=getdist; print "testing"; if ((d>20) && (d<50)) print "in range"; print "hopping"; froghop;};

//candylift commands
//pushup, dropdown
//function upanddown {pushup; delay(1000); dropdown;}
//function updntrig {if(d4==1)upDn;}
//function startup {run upanddown,5000;}

//relay functions
//openrelay, closerelay
//function runfan {closerelay; delay(2000); openrelay;}

//LED commands
//setcolor

#include "bitlash.h"
#include <Servo.h>

//set distance sensor pins
#define TRIG_PIN 7 
#define ECHO_PIN 8 

// define RGB pinouts
#define REDPINOUT 15
#define GREENPINOUT 14
#define BLUEPINOUT 16

//define relay pin
#define RELAY_PIN 4

//set Candy servo pin
#define CANDY_SERVO_PIN 5
#define CANDY_TOP 50
#define CANDY_BOTTOM 120

//set Frog servo pin
#define FROG_SERVO_PIN 3

Servo candyServo;
Servo frogServo;

//--------------------------------------------------------------
numvar setcolor(void) {
  int iRedLevel = getarg(1);
  int iGreenLevel = getarg(2);
  int iBlueLevel = getarg(3);

  Serial.print("changing color to r:"); Serial.print(iRedLevel);
  Serial.print(",g:"); Serial.print(iGreenLevel);
  Serial.print(",b:"); Serial.println(iBlueLevel);

  iRedLevel = 255 - iRedLevel;
  iGreenLevel = 255 - iGreenLevel;
  iBlueLevel = 255 - iBlueLevel;
  
  analogWrite(REDPINOUT, iRedLevel);
  analogWrite(GREENPINOUT, iGreenLevel);
  analogWrite(BLUEPINOUT, iBlueLevel);
}

//--------------------------------------------------------------
numvar pushup(void) {
  for (byte pos=CANDY_BOTTOM; pos>=CANDY_TOP; pos--)     // goes from 180 degrees to 0 degrees 
  {                                
    candyServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}

//--------------------------------------------------------------
numvar dropdown(void) {
    candyServo.write(CANDY_BOTTOM);              // tell servo to go to position in variable 'pos' 
}

//-------------------------------------------------------------------
numvar turnquick(void) {
  frogServo.writeMicroseconds(getarg(1));
  return getarg(1);
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

  Serial.print("distance: "); Serial.println(distance);
  return distance;
}

//-------------------------------------------------------------------
numvar closerelay(void) {
  Serial.println("closing relay");
  digitalWrite(RELAY_PIN, LOW);
}

//-------------------------------------------------------------------
numvar openrelay(void) {
  Serial.println("opening relay");
  digitalWrite(RELAY_PIN, HIGH);
}

//-------------------------------------------------------------------
void setup(void) {
  pinMode(19,OUTPUT);
  
   //setup RGB output pins
  pinMode(REDPINOUT, OUTPUT);
  pinMode(GREENPINOUT, OUTPUT);
  pinMode(BLUEPINOUT, OUTPUT);

  pinMode(RELAY_PIN,OUTPUT);
  digitalWrite(RELAY_PIN, HIGH);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(RELAY_PIN, HIGH);
  
  // initialize bitlash and set primary serial port baud
  // print startup banner and run the startup macro
  initBitlash(57600);

  // initialize ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  candyServo.attach(CANDY_SERVO_PIN);
  candyServo.write(CANDY_BOTTOM);
  frogServo.attach(FROG_SERVO_PIN);
  frogServo.writeMicroseconds(1500);
  
  // add custom function to push candy stick up
  addBitlashFunction("pushup", (bitlash_function) pushup);
  // add custom function to drop candy stick down
  addBitlashFunction("dropdown", (bitlash_function) dropdown);
  // add custom function to control servo
  addBitlashFunction("turnquick", (bitlash_function) turnquick);
  // add custom function to control ultrasonic sensor
  addBitlashFunction("getdist", (bitlash_function) getdist);
  // add custom function to open the relay
  addBitlashFunction("openrelay", (bitlash_function) openrelay);
  // add custom function to close the relay
  addBitlashFunction("closerelay", (bitlash_function) closerelay);
  // add custom function to set the LED color
  addBitlashFunction("setcolor", (bitlash_function) setcolor);
}

//-------------------------------------------------------------------
void loop(void) {
  runBitlash();
}
