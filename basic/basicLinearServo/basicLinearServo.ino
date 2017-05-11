#include <Servo.h>

//set Candy servo pin
#define CANDY_SERVO_PIN 5
#define CANDY_TOP 50
#define CANDY_BOTTOM 120

Servo candyServo;

//--------------------------------------------------------------
//numvar pushup(void) {
//--------------------------------------------------------------
void pushup(void) {
  Serial.println("basicLinearServo.pushup");
  for (byte pos=CANDY_BOTTOM; pos>=CANDY_TOP; pos--)     // goes from 180 degrees to 0 degrees 
  {                                
    candyServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}

//--------------------------------------------------------------
//numvar dropdown(void) {
//--------------------------------------------------------------
void dropdown(void) {
  Serial.println("basicLinearServo.dropdown");
  candyServo.write(CANDY_BOTTOM);              // tell servo to go to position in variable 'pos' 
}

//--------------------------------------------------------------
// this function will be implemented in bitlash as the IR function
// for the candy store window.
//--------------------------------------------------------------
void upanddown() {
  Serial.println("basicLinearServo.upanddown");
  pushup();
  delay(1000);
  dropdown();  
}

//--------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("basicLinearServo.setup");

  candyServo.attach(CANDY_SERVO_PIN);
  candyServo.write(CANDY_BOTTOM);
}

//--------------------------------------------------------------
void loop() {
  Serial.println("basicLinearServo.loop");
  upanddown();
  delay(1000);
}
