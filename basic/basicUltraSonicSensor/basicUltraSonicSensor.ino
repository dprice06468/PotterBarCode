//set distance sensor pins
#define TRIG_PIN 7 
#define ECHO_PIN 8 

//-------------------------------------------------------------------
//numvar getdist(void) {
//-------------------------------------------------------------------
int getdist(void) {
  long duration;
  int distance;
  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  //distance = (duration/2) / 29.1;
  distance = duration*0.034/2;

//  Serial.print("distance: "); Serial.println(distance);
  return distance;
}

//-------------------------------------------------------------------
void setup() {
  Serial.begin(9600);
  Serial.println("basicUltraSonicSensor.setup");

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

//-------------------------------------------------------------------
void loop() {
  Serial.println("basicUltraSonicSensor.loop");
  Serial.print("Distance: ");
  Serial.println(getdist());
  delay(1000);
}
