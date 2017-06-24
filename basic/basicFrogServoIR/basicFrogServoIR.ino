#include <IRremote.h>
#include <Servo.h>

//set Frog servo pin
#define FROG_SERVO_PIN 13

Servo frogServo;

#define LEDPIN 1
#define IRRECVPIN 3 //PIN receiving signal from IR (moteino 9, mega 3)
#define IR_ENTER 0x219E10EF

IRrecv irrecv(IRRECVPIN);
decode_results results;

//----------------------------------------------------------------------
//numvar checkIR(void) {
//----------------------------------------------------------------------
int checkir() {
  Serial.println("basicIR.checkir");
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
      irrecv.resume(); 
    if (results.value == IR_ENTER) {
//      irrecv.resume(); 
      return 1;
    }
    // Receive the next value
//    irrecv.resume(); 
  }
  return 0;
}

//-------------------------------------------------------------------
//numvar turnquick(void) {
//  frogServo.writeMicroseconds(getarg(1));
//  return getarg(1);
//}
//-------------------------------------------------------------------
int turnquick(int ms) {
  Serial.println("basicFrogServo.turnquick");
  frogServo.writeMicroseconds(ms);
  return ms;
}

//-------------------------------------------------------------------
// this function will be implemented in bitlash as candystore
// proximity action
//-------------------------------------------------------------------
void froghop() {
  Serial.println("basicFrogServo.froghop");
  frogServo.attach(FROG_SERVO_PIN);
  turnquick(1200); 
  delay(200); 
  turnquick(1700);
  frogServo.detach();
}

//-------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("basicFrogServo.setup");

  frogServo.attach(FROG_SERVO_PIN);
  frogServo.writeMicroseconds(1500);
  frogServo.detach();

  irrecv.enableIRIn();
  
  // initialize output pins.
  pinMode(LEDPIN, OUTPUT);
}

//-------------------------------------------------------------------
void loop() {
  if (checkir()) {
    digitalWrite(LEDPIN, HIGH);
    delay(2000);
    digitalWrite(LEDPIN, LOW);
  }
  froghop();
  delay(2000);
}
