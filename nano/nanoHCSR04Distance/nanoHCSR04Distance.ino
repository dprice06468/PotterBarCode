int trig = 2; 
int echo = 4; 

void setup() 
{  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
}

void loop() {
  long duration, distance;
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;

//  Serial.print("                ");
  Serial.print(distance);
  Serial.println(" cm");
  delay(250); 
}

