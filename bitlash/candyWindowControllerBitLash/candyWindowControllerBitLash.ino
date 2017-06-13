//contols frog, candy, and sound based on proximity and IR sensors

//turnquick, getdist
//function froghop {playtrak(3); hop;}
//function hop {turnquick(1200); delay(200); turnquick(1700); snooze(5000);}
//function raisecandy {print "raise candy";pushup;snooze(2000);dropdown;}
//function candyspell {playtrak(4); raisecandy};
//function prxaction {froghop;}
//function iraction {raisecandy;}
//playtrak(#)

#include "IRremote.h"
#include "bitlash.h"
#include "Servo.h"
#include "SoftwareSerial.h"
#include "Adafruit_Soundboard.h"

//#define DEBUG
//#define DISTANCE

//--- configure sound pins ----
#define SFX_TX 29  //moteino 9 mega 29
#define SFX_RX 30  //moteino 7 mega 30
#define SFX_RST 28  //moteino 4 mega 28

// we'll be using software serial
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

// pass the software serial to Adafruit_soundboard, the second
// argument is the debug port (not used really) and the third 
// arg is the reset pin
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

//--- configure IR ------------
//set IR pins
#define LED_PIN 1
#define IR_RECV_PIN 3

IRrecv irrecv(IR_RECV_PIN);
decode_results results;
//----------------------------

//--- configure frog servo ---
//set servo pin
#define FROG_SERVO_PIN 13  //moteino=9, mega=13
//set distance sensor pins
#define TRIG_PIN 18  //moteino=7, mega=18
#define ECHO_PIN 19  //moteino=8, mega=19

short firsttime = 1;
Servo frogServo;
//----------------------------

//--- configure candy servo ---
#define CANDY_SERVO_PIN 12
#define CANDY_TOP 50
#define CANDY_BOTTOM 120

Servo candyServo;
//-----------------------------

//-------------------------------------------------------------------
numvar ping(void) {
  return TCNT1; // return the value of Timer 1
}

//-------------------------------------------------------------------
numvar playtrak(void) {
  Serial.print("\nPlaying track #"); Serial.println(getarg(1));
  if (! sfx.playTrack(getarg(1)) ) {
    Serial.println("Failed to play track?");
  }
}

//-------------------------------------------------------------------
int checkir(void) {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    if (results.value == 0x219E10EF) {
      irrecv.resume(); 
      return 1;
    }
    irrecv.resume(); // Receive the next value
  }
  return 0;
}

//--------------------------------------------------------------
numvar pushup(void) {
  Serial.println("basicLinearServo.pushup");
  for (byte pos=CANDY_BOTTOM; pos>=CANDY_TOP; pos--)     // goes from 180 degrees to 0 degrees 
  {                                
    candyServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}

//--------------------------------------------------------------
numvar dropdown(void) {
  Serial.println("basicLinearServo.dropdown");
  candyServo.write(CANDY_BOTTOM);              // tell servo to go to position in variable 'pos' 
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

  //enable the IR sensor
  irrecv.enableIRIn();
  
  // initialize LED pin
  pinMode(LED_PIN, OUTPUT);

  // initialize ultrasonic sensor
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // initialize servo
  frogServo.attach(FROG_SERVO_PIN);
  frogServo.writeMicroseconds(1500);

  //initialize candy servo
  candyServo.attach(CANDY_SERVO_PIN);
  candyServo.write(CANDY_BOTTOM);

  //initialize SoundFX board
  ss.begin(9600);
  if (!sfx.reset()) {
    Serial.println("Not found");
    while (1);
  }
  Serial.println("SFX board found");
  
  // add custom function to handle ping
  addBitlashFunction("ping", (bitlash_function) ping);
  // add custom function to control servo
  addBitlashFunction("turnquick", (bitlash_function) turnquick);
  // add custom function to push candy stick up
  addBitlashFunction("pushup", (bitlash_function) pushup);
  // add custom function to drop candy stick down
  addBitlashFunction("dropdown", (bitlash_function) dropdown);
  // add custom function to play a sound track
  addBitlashFunction("playtrak", (bitlash_function) playtrak);
}

//-------------------------------------------------------------------
void loop() {
  if (firsttime) {
    firsttime = 0;
    assignVar('l'-'a', millis());
    assignVar('r'-'a', (random(7)+3)*1000);
  }
  long dist = getdist(); 

  //check for IR spell.  if found, fire spell
  if (checkir()) doCommand("iraction");
  
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
  if (getVar('x'-'a') && getVar('t'-'a')) doCommand("prxaction");
  
  //run bitlash functions
  runBitlash();
}
