int trig = 2; 
int echo = 4; 
int minTriggerDistance = 20;
int maxTriggerDistance = 50;
char actions[4] = {'A','B','C','D'};
long waitBetweenActions = 5000;
long lastActionTime = millis();
long randomAction, duration, distance;

void setup() 
{  
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(9600);
  randomSeed(analogRead(A0));
  Serial.println("nanoHCSR04");
  Serial.print("Will trigger action when object within ");
  Serial.print(minTriggerDistance);
  Serial.print("-");
  Serial.print(maxTriggerDistance);
  Serial.println(" cm.");
  Serial.print("Will reset trigger ");
  Serial.print(waitBetweenActions/1000);
  Serial.println(" sec after previous action.");
}

void loop() {
//  Serial.print("last: ");
//  Serial.print(lastActionTime);
//  Serial.print(" current: ");
//  Serial.println(millis());

  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH);
  distance = (duration/2) / 29.1;

  if ((distance > minTriggerDistance) && (distance < maxTriggerDistance)) {
    if (millis() > lastActionTime + waitBetweenActions) {
      randomAction = random(0,4);
      Serial.print(distance);
      Serial.print(" cm,");
      Serial.print(" Action: ");
      Serial.println(actions[randomAction]);

      lastActionTime = millis();
    }
  }

  delay(1000); 
}

