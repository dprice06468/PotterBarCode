#include <IRremote.h>
#include <IRremoteInt.h>
#include <BitVoicer11.h>

/*
 */
char reader;
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
  Serial1.begin(9600);
  irrecv.enableIRIn();
  
  // initialize output pins.
  pinMode(pinLeft, OUTPUT);
  pinMode(pinCenter, OUTPUT);
  pinMode(pinRight, OUTPUT);
  pinMode(pinLED, OUTPUT);
  //digitalWrite(pinLED, HIGH);
  //delay(1000);
  //digitalWrite(pinLED, LOW);
}

//----------------------------------------------------------------------
// the loop function runs over and over again forever
//----------------------------------------------------------------------
void loop() {
  if (Serial1.available()>0)
  {
    char val = Serial1.read();
    if (val == '$') {
      delay(50);
      Serial.println("found wifi input");
      val = Serial1.read();
      Serial.println(val);
      if (val == '1') digitalWrite(pinLED, HIGH);
      if (val == '2') digitalWrite(pinLED, LOW);
      Serial1.flush();
    }
  }

  if (Serial.available()>0)
  {
    char val = Serial.read();
//    Serial.println(val);
    if (val == '$') {
      delay(50);
      Serial.println("found input");
      val = Serial.read();
      Serial.println(val);
      if (val == '1') digitalWrite(pinLED, HIGH);
      if (val == '2') digitalWrite(pinLED, LOW);
      Serial.flush();
    }
  }
/*  
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
  }*/
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
  if (strSpell == "lefton") 
    leftOnSpell();
    //digitalWrite(pinLeft, HIGH);   
  else if (strSpell == "leftoff") 
    leftOffSpell();
    //digitalWrite(pinLeft, LOW);    
  else if (strSpell == "center") 
    centerSpell();
    //{
    //digitalWrite(pinCenter, HIGH); 
    //delay(3000);                   
    //digitalWrite(pinCenter, LOW);  
    //}
  else if (strSpell == "right") 
    rightSpell();
    //{
    //digitalWrite(pinRight, HIGH);  
    //delay(3000);                   
    //digitalWrite(pinRight, LOW);   
    //}
}

void leftOnSpell() {
    digitalWrite(pinLeft, HIGH);   
}

void leftOffSpell() {
    digitalWrite(pinLeft, LOW);   
}

void centerSpell() {
    digitalWrite(pinCenter, HIGH); 
    delay(3000);                   
    digitalWrite(pinCenter, LOW);  
}

void rightSpell() {
    digitalWrite(pinRight, HIGH);  
    delay(3000);                   
    digitalWrite(pinRight, LOW);   
}

