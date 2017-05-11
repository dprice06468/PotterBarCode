#include <IRremote.h>
#include <IRremoteInt.h>
/*
 */

int pinLED = 3;
int recvPin = 5;        //PIN receiving signal from IR

IRrecv irrecv(recvPin);
decode_results results;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn();
  
  // initialize output pins.
  pinMode(pinLED, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  if(irrecv.decode(&results)) {
    //Serial.println(results.value, HEX);
    if (results.value == 0x219E10EF) {
      digitalWrite(pinLED, HIGH);
      delay(3000);                   // wait for a second
      digitalWrite(pinLED, LOW);    // turn the LED off by making the voltage LOW
    }  
    irrecv.resume(); // Receive the next value
  }
}

