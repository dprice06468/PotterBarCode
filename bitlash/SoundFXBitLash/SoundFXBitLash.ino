//playtrak(#)

#include "bitlash.h"
#include "SoftwareSerial.h"
#include "Adafruit_Soundboard.h"

// Choose any two pins that can be used with SoftwareSerial to RX & TX
#define SFX_TX 29  //moteino 9 mega 29
#define SFX_RX 30  //moteino 7 mega 30

// Connect to the RST pin on the Sound Board
#define SFX_RST 28  //moteino 4 mega 28

// You can also monitor the ACT pin for when audio is playing!

// we'll be using software serial
SoftwareSerial ss = SoftwareSerial(SFX_TX, SFX_RX);

// pass the software serial to Adafruit_soundboard, the second
// argument is the debug port (not used really) and the third 
// arg is the reset pin
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, SFX_RST);

//-------------------------------------------------------------------
numvar ping(void) {
  return TCNT1; // return the value of Timer 1
}

//--------------------------------------------------------------
numvar playtrak(void) {
  Serial.print("\nPlaying track #"); Serial.println(getarg(1));
  if (! sfx.playTrack(getarg(1)) ) {
    Serial.println("Failed to play track?");
  }
}

//--------------------------------------------------------------
numvar playname(void) {
  Serial.print("\nPlaying track named: "); Serial.println(getarg(1));
}

//-------------------------------------------------------------------
void setup() {
  // initialize bitlash and set primary serial port baud
  // print startup banner and run the startup macro
  initBitlash(57600);

  Serial.println("Adafruit Sound Board!");
  
  // softwareserial at 9600 baud
  ss.begin(9600);
  // can also do Serial1.begin(9600)

  if (!sfx.reset()) {
    Serial.println("Not found");
    while (1);
  }
  Serial.println("SFX board found");
  
  // add custom function to handle ping
  addBitlashFunction("ping", (bitlash_function) ping);
  // add custom function to push candy stick up
  addBitlashFunction("playtrak", (bitlash_function) playtrak);
  // add custom function to drop candy stick down
  addBitlashFunction("playname", (bitlash_function) playname);
}

//-------------------------------------------------------------------
void loop() {
  runBitlash();
}
