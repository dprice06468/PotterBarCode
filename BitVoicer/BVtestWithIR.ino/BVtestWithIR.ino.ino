#include <IRremote.h>
#include <IRremoteInt.h>
#include <BitVoicer11.h>

/*
 */

int pinLeft = 13;
int pinCenter = 12;
int pinRight = 11;
int pinLED = 3;
int recvPin = 5;        //PIN receiving signal from IR
unsigned long startTime;
String currentSpell;
boolean wasSpellSpoken = false;
boolean wasWandWaved = false;

IRrecv irrecv(recvPin);
decode_results results;

BitVoicerSerial bitVoicer = BitVoicerSerial();
byte dataType = BV_STR;

//----------------------------------------------------------------------
// the setup function runs once when you press reset or power the board
//----------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  
  // initialize output pins.
  pinMode(pinLeft, OUTPUT);
  pinMode(pinCenter, OUTPUT);
  pinMode(pinRight, OUTPUT);
  pinMode(pinLED, OUTPUT);
}

//----------------------------------------------------------------------
// the loop function runs over and over again forever
//----------------------------------------------------------------------
void loop() {
  if (irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    if (results.value == 0x219E10EF) {
      wasWandWaved = true;
      digitalWrite(pinLED, HIGH);
      startTime = millis();
    }    
    // Receive the next value
    irrecv.resume(); 
  } else {
    if ((startTime > 0) && (millis() - startTime > 3000))
      resetTimer();     
  }

  if (wasSpellSpoken && wasWandWaved) {
    invokeSpell(currentSpell);
    resetTimer();     
  }
}

//----------------------------------------------------------------------
// this function resets the timer and settings when the timer expires
//----------------------------------------------------------------------
void resetTimer() {
  digitalWrite(pinLED, LOW);
  wasSpellSpoken = false;
  wasWandWaved = false;
  startTime = 0;
}

//----------------------------------------------------------------------
// this function is fired whenever a spell is spoken
//----------------------------------------------------------------------
void serialEvent() {
  bitVoicer.getData();

  currentSpell = bitVoicer.strData;

  if (currentSpell == "") 
    return;
  
  digitalWrite(pinLED, HIGH);
  wasSpellSpoken = true;
  startTime = millis();
}

//----------------------------------------------------------------------
// this function executes the spoken spell
//----------------------------------------------------------------------
void invokeSpell(String strSpell) {
  if (strSpell == "left") {
    digitalWrite(pinLeft, HIGH);   
    delay(3000);                   
    digitalWrite(pinLeft, LOW);    
  } else if (strSpell == "center") {
    digitalWrite(pinCenter, HIGH); 
    delay(3000);                   
    digitalWrite(pinCenter, LOW);  
  } else if (strSpell == "right") {
    digitalWrite(pinRight, HIGH);  
    delay(3000);                   
    digitalWrite(pinRight, LOW);   
  }
}

