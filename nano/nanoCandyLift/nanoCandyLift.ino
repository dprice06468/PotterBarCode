#include <Servo.h>

#define SERVO_PIN 9
#define TRIGGER_PIN 4
#define TOP 50
#define BOTTOM 120

Servo myServo;

void setup() {
  Serial.begin(9600);
  Serial.println("nanoCanyLift");
  myServo.attach(SERVO_PIN);
  myServo.write(BOTTOM);
  
  pinMode(TRIGGER_PIN, INPUT);
}

void loop() {
  if (digitalRead(TRIGGER_PIN) == HIGH)
  {
        pushUp();
        delay(2000);
        dropDown();
        delay(5000);
  }

  if (Serial.available() > 0)
  {
    char dir = Serial.read();

    switch (dir) {
      case 'u':
        pushUp();
        break;
      case 'd':
        dropDown();
        break;
      case 'b':
        pushUp();
        dropDown();
        break;
    }
  }
}
//--------------------------------------------------------------
void pushUp() {
//  for (byte pos=180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  for (byte pos=BOTTOM; pos>=TOP; pos--)     // goes from 180 degrees to 0 degrees 
  {                                
    myServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}
//--------------------------------------------------------------
void dropDown() {
//    myServo.write(180);              // tell servo to go to position in variable 'pos' 
    myServo.write(BOTTOM);              // tell servo to go to position in variable 'pos' 
}


