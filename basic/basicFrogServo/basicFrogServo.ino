#include <Servo.h>

//set Frog servo pin
#define FROG_SERVO_PIN 3

Servo frogServo;

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
  turnquick(1200); 
  delay(200); 
  turnquick(1500);
}

//-------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("basicFrogServo.setup");

  frogServo.attach(FROG_SERVO_PIN);
  frogServo.writeMicroseconds(1500);
}

//-------------------------------------------------------------------
void loop() {
  froghop();
  delay(2000);
}
