#include <Servo.h>

#define SERVO_PIN 9

Servo myServo;

void setup() {
  myServo.attach(SERVO_PIN);
}

void loop() {
//  myServo.writeMicroseconds(1500);
//  delay(1000);
//  myServo.writeMicroseconds(600);
//  delay(1000);
  myServo.writeMicroseconds(1500);
  delay(1000);
  myServo.writeMicroseconds(1200);
  delay(150);
  myServo.writeMicroseconds(1500);
  delay(150);
  myServo.writeMicroseconds(1200);
  delay(150);
  myServo.writeMicroseconds(1500);
  delay(150);
  myServo.writeMicroseconds(1200);
  delay(150);
  myServo.writeMicroseconds(1500);
  delay(150);
  myServo.writeMicroseconds(1200);
  delay(150);
  myServo.writeMicroseconds(1500);
  delay(1000);
}
