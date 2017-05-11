 /* 
  Connect 5V on Arduino to VCC on Relay Module
  Connect GND on Arduino to GND on Relay Module 
  Connect GND on Arduino to the Common Terminal (middle terminal) on Relay Module. */
 
 #define RELAY_PIN 4   // Connect Digital Pin 4 on Arduino to RELAY_PIN on Relay Module
 
//-------------------------------------------------------------------
//numvar closerelay(void) {
//-------------------------------------------------------------------
void closerelay() {
  Serial.println("closing relay");
  digitalWrite(RELAY_PIN, LOW);
}

//-------------------------------------------------------------------
//numvar openrelay(void) {
//-------------------------------------------------------------------
void openrelay() {
  Serial.println("opening relay");
  digitalWrite(RELAY_PIN, HIGH);
}

//-------------------------------------------------------------------
 void setup(){
   Serial.begin(9600);
   Serial.println("basicRelay.setup");
   
   pinMode(RELAY_PIN, OUTPUT);
   
   closerelay();
   delay(2000); //Wait 2 seconds before starting sequence
 }
 
//-------------------------------------------------------------------
 void loop(){
   Serial.println("basicRelay.loop");
   openrelay();
   delay(1000);
   closerelay();
   delay(1000);
 }
