//openrelay
//closerelay
//function closeopen {closerelay;delay(2000);openrelay;}
//function toggle9 {d15=!d15;}

#include "bitlash.h"

#define RELAY_PIN 17   // Pin 4 (moteino) 17 (motenio mega) RELAY_PIN on Relay Module

//-------------------------------------------------------------------
numvar ping(void) {
  return TCNT1; // return the value of Timer 1
}

//-------------------------------------------------------------------
numvar closerelay(void) {
  Serial.println("closing relay");
  digitalWrite(RELAY_PIN, HIGH);
}

//-------------------------------------------------------------------
numvar openrelay(void) {
  Serial.println("opening relay");
  digitalWrite(RELAY_PIN, LOW);
}

//----------------------------------------------------------------------
void setup() {
  // initialize bitlash and set primary serial port baud
  // print startup banner and run the startup macro
  initBitlash(57600);

  Serial.println("basicRelay.setup");
   
  // initialize output pins.
  pinMode(RELAY_PIN, OUTPUT);
   
  // add custom function to handle ping
  addBitlashFunction("ping", (bitlash_function) ping);
  // add custom function to open the relay
  addBitlashFunction("openrelay", (bitlash_function) openrelay);
  // add custom function to close the relay
  addBitlashFunction("closerelay", (bitlash_function) closerelay);
}

//----------------------------------------------------------------------
void loop() {
  runBitlash();
}

