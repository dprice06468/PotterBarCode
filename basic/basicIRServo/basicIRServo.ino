// Written by: Mohamed Soliman
// This code is for controlling servo motor with IR remote control
// When clicking at any of two buttons the motor is toggling between the rotation and stop

//#define __AVR_ATmega1280__
#include <IRremote.h>      //must copy IRremote library to arduino libraries
#include <Servo.h>
#define plus 0x219E906F   //clockwise rotation button
#define minus 0x219EE01F  //counter clockwise rotation button

int RECV_PIN = 3;       //IR receiver pin
Servo servo;
int val;                //rotation angle
bool cwRotation, ccwRotation;  //the states of rotation

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup()
{
  Serial.begin(9600);
#ifdef __AVR_ATmega1280__
Serial.println("__AVR_ATmega1280__");
#endif

  irrecv.enableIRIn(); // Start the receiver
  servo.attach(13);     //servo pin
}

void loop() 
{
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value

    if (results.value == plus)
    {
      cwRotation = !cwRotation;      //toggle the rotation value
      ccwRotation = false;         //no rotation in this direction
    }

    if (results.value == minus)
    {
      ccwRotation = !ccwRotation;   //toggle the rotation value
      cwRotation = false;            //no rotation in this direction
    }
  }
  if (cwRotation && (val != 175))  {
    val++;                         //for colockwise button
  }
  if (ccwRotation && (val != 0))  {
    val--;                         //for counter colockwise button
  }
//  servo.attach(13); 
//  delay(100);
  servo.write(val);
//  delay(100);
//  servo.detach();     
//  delay(100);
  delay(20);          //General speed
}

