//checkir
//openrelay
//closerelay
//function startup {run checkact;}
//function checkact {if (checkir()==1) iraction; snooze(1000);}
//function iraction {d1=15;closeopen;d15=0;}
//function closeopen {closerelay;delay(2000);openrelay;}
//function toggle9 {d15=!d15;}

#include <IRremote.h>
#include "bitlash.h"

#define LED_PIN 1     //moteino 9 mega 1
#define RECV_PIN 3    //PIN receiving signal from IR moteino 5 mega 3
#define RELAY_PIN 17  // Pin 4 (moteino) 17 (motenio mega) RELAY_PIN on Relay Module

IRrecv irrecv(RECV_PIN);
decode_results results;

//-------------------------------------------------------------------
numvar ping(void) {
  return TCNT1; // return the value of Timer 1
}

//----------------------------------------------------------------------
numvar checkir(void) {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    if (results.value == 0x219E10EF) {
      irrecv.resume(); 
      return 1;
    }
    // Receive the next value
    irrecv.resume(); 
  }
  return 0;
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
   
  irrecv.enableIRIn();
  
  // initialize output pins.
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  // add custom function to handle ping
  addBitlashFunction("ping", (bitlash_function) ping);
  // add custom function to handle an IR signal
  addBitlashFunction("checkir", (bitlash_function) checkir);
  // add custom function to open the relay
  addBitlashFunction("openrelay", (bitlash_function) openrelay);
  // add custom function to close the relay
  addBitlashFunction("closerelay", (bitlash_function) closerelay);
}

//----------------------------------------------------------------------
void loop() {
  //check for IR spell.  if found, fire spell
  if (checkir()) doCommand("iraction");
  
  runBitlash();
}

