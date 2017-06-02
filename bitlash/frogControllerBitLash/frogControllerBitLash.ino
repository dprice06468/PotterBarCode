//if user within sensor range make frog hop randomly

//turnquick, getdist
//function froghop1 {if (x && t) froghop; snooze(1000);};
//function froghop {turnquick(1200); delay(200); turnquick(1700); snooze(5000);};

#include "bitlash.h"
#include <Servo.h>

//#define DEBUG
//#define DISTANCE

//set servo pin
#define FROG_SERVO_PIN 13  //moteino=9, mega=13
//set distance sensor pins
#define TRIG_PIN 18  //moteino=7, mega=18
#define ECHO_PIN 19  //moteino=8, mega=19

short firsttime = 1;
Servo frogServo;

//-------------------------------------------------------------------
numvar ping(void) {
  return TCNT1; // return the value of Timer 1
}

//-------------------------------------------------------------------
int getdist(void) {
  long distance, duration;
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration*0.034/2;
//  distance = (duration/2) / 29.1;
  
  #ifdef DISTANCE
    Serial.print("distance: "); Serial.println(distance);
  #endif
  
  return distance;
}

//-------------------------------------------------------------------
int isTTR(void) {
  long cur = millis(); 

  if (cur-getVar('l'-'a') > getVar('r'-'a')) {
    #ifdef DEBUG
      Serial.print("old l: ");Serial.print(getVar('l'-'a'));Serial.print(" c: ");Serial.print(cur);Serial.print(" r: ");Serial.print(getVar('r'-'a'));
    #endif
    assignVar('l'-'a', millis()); 
    assignVar('r'-'a', (random(7)+3)*1000); 
    #ifdef DEBUG
      Serial.print(" new l: ");Serial.print(getVar('l'-'a'));Serial.print(" new r: ");Serial.println(getVar('r'-'a'));
    #endif
    return 1;
  }
  else 
    return 0;
}

//-------------------------------------------------------------------
numvar turnquick(void) {
  frogServo.writeMicroseconds(getarg(1));
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
  frogServo.attach(FROG_SERVO_PIN);
  frogServo.writeMicroseconds(1500);

  // add custom function to handle ping
  addBitlashFunction("ping", (bitlash_function) ping);
  // add custom function to control servo
  addBitlashFunction("turnquick", (bitlash_function) turnquick);
}

//-------------------------------------------------------------------
void loop() {
  if (firsttime) {
    firsttime = 0;
    assignVar('l'-'a', millis());
    assignVar('r'-'a', (random(7)+3)*1000);
  }
  long dist = getdist(); 

  //check to see if someone is in range
  if ((dist > 20) && (dist < 50)) {
    assignVar('x'-'a', 1);  //set x var to indicate in proximity

    //determine if it is time to run
    if (isTTR()) 
      assignVar('t'-'a', 1);
    else
      assignVar('t'-'a', 0);
  } else {
    assignVar('x'-'a', 0);  //set x var to indicate not in proximity
    //assignVar('r'-'a', 0);
  }

  //if (getVar('x'-'a') && getVar('t'-'a')) Serial.println("hop");
  if (getVar('x'-'a') && getVar('t'-'a')) doCommand("froghop");
  
  //run bitlash functions
  runBitlash();
}
