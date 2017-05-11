#include <Servo.h>

Servo myservo;  // create servo object to control a servo 
                // a maximum of eight servo objects can be created 

int pos = 0;    // variable to store the servo position 

void setup() 
{ 
  myservo.attach(9);  // attaches the servo on pin 0 to the servo object 
//  myservo.write(180);              // tell servo to go to position in variable 'pos' 
//  delay(1000);
//  myservo.write(0);              // tell servo to go to position in variable 'pos' 
//  delay(1000);
//  myservo.write(180);              // tell servo to go to position in variable 'pos' 
} 


void loop() 
{ 
  for(pos = 180; pos>=135; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  //delay(3000);
  for(pos = 135; pos < 180; pos += 5)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  //delay(3000);
}
