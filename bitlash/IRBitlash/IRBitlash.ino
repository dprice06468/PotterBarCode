//checkir
//function startup {run checkact;}
//function checkact {if (checkir()==1) iraction; snooze(1000);}
//function iraction {d9=1;delay(2000);d9=0;}
//function toggle9 {d9=!d9;}

#include <IRremote.h>
//#include <IRremoteInt.h>
#include "bitlash.h"

int pinLED = 9;
int recvPin = 5;        //PIN receiving signal from IR

IRrecv irrecv(recvPin);
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

//----------------------------------------------------------------------
void setup() {
  // initialize bitlash and set primary serial port baud
  // print startup banner and run the startup macro
  initBitlash(57600);

  irrecv.enableIRIn();
  
  // initialize output pins.
  pinMode(pinLED, OUTPUT);

  // add custom function to handle ping
  addBitlashFunction("ping", (bitlash_function) ping);
  // add custom function to handle an IR signal
  addBitlashFunction("checkir", (bitlash_function) checkir);
}

//----------------------------------------------------------------------
void loop() {
  runBitlash();
}

