// requires pwm output pins to led strip

// define RGB pinouts
#define REDPINOUT 15
#define GREENPINOUT 14
#define BLUEPINOUT 16

//--------------------------------------------------------------
//numvar setcolor(void) {
//  int iRedLevel = getarg(1);
//  int iGreenLevel = getarg(2);
//  int iBlueLevel = getarg(3);
//--------------------------------------------------------------
void setcolor(int iRedLevel, int iGreenLevel, int iBlueLevel) {
  Serial.print("changing color to r:"); Serial.print(iRedLevel);
  Serial.print(",g:"); Serial.print(iGreenLevel);
  Serial.print(",b:"); Serial.println(iBlueLevel);

  iRedLevel = 255 - iRedLevel;
  iGreenLevel = 255 - iGreenLevel;
  iBlueLevel = 255 - iBlueLevel;
  
  analogWrite(REDPINOUT, iRedLevel);
  analogWrite(GREENPINOUT, iGreenLevel);
  analogWrite(BLUEPINOUT, iBlueLevel);
}

//--------------------------------------------------------------
void setup() {
  Serial.begin(9600);

  //setup RGB output pins
  pinMode(REDPINOUT, OUTPUT);
  pinMode(GREENPINOUT, OUTPUT);
  pinMode(BLUEPINOUT, OUTPUT);
}

//--------------------------------------------------------------
void loop() {
  int i;
  for (i=0; i<256; i++) setcolor(i,0,0);
  for (i=0; i<256; i++) setcolor(0,i,0);
  for (i=0; i<256; i++) setcolor(0,0,i);
}
