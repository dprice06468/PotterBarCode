#include <IRremote.h>

int pinLED = 21;
int recvPin = 9;        //PIN receiving signal from IR
boolean scriptsLoaded = false;  //variable to indicate whether scripts have been loaded

IRrecv irrecv(recvPin);
decode_results results;

//----------------------------------------------------------------------
//numvar checkIR(void) {
//----------------------------------------------------------------------
int checkir() {
  Serial.println("basicIR.checkir");
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
  Serial.begin(9600);
  Serial.println("basicIR.setup");
  
  irrecv.enableIRIn();
  
  // initialize output pins.
  pinMode(pinLED, OUTPUT);
}

//----------------------------------------------------------------------
void loop() {
  Serial.println("basicIR.loop");
  if (checkir) {
    digitalWrite(pinLED, HIGH);
    delay(2000);
    digitalWrite(pinLED, LOW);
  }
}
