#include <BitVoicer11.h>

/*
 */

int pinLeft = 13;
int pinCenter = 12;
int pinRight = 11;

BitVoicerSerial bitVoicer = BitVoicerSerial();
byte dataType = BV_STR;

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  
  // initialize output pins.
  pinMode(pinLeft, OUTPUT);
  pinMode(pinCenter, OUTPUT);
  pinMode(pinRight, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
}

void serialEvent() {
  bitVoicer.getData();

  if (bitVoicer.strData == "") 
    return;
  
  if (bitVoicer.strData == "left") {
    digitalWrite(pinLeft, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(3000);                   // wait for a second
    digitalWrite(pinLeft, LOW);    // turn the LED off by making the voltage LOW
  } else if (bitVoicer.strData == "center") {
    digitalWrite(pinCenter, HIGH); // turn the LED on (HIGH is the voltage level)
    delay(3000);                   // wait for a second
    digitalWrite(pinCenter, LOW);  // turn the LED off by making the voltage LOW
  } else if (bitVoicer.strData == "right") {
    digitalWrite(pinRight, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(3000);                   // wait for a second
    digitalWrite(pinRight, LOW);   // turn the LED off by making the voltage LOW
  }
}
