//trigger a relay given serial input
//send a $1 to Rx to close the relay
//send a $2 to Rx to open the relay

int pinRelay = 9;  //relay trigger pin

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);

  // initialize digital pin 13 as an output.
  pinMode(pinRelay, OUTPUT);
  digitalWrite(pinRelay, HIGH);       // open the relay
}

// the loop function runs over and over again forever
void loop() {
  if (Serial.available()>0)
  {
    char val = Serial.read();
    if (val == '$') {
      delay(50);
      val = Serial.read();
      Serial.println("found input: " + val);

      if (val == '1') digitalWrite(pinRelay, LOW);   //close the relay
      if (val == '2') digitalWrite(pinRelay, HIGH);  //open the relay
      Serial.flush();
    }
  }
}
