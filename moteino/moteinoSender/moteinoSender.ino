#include <RFM69.h>    //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>//get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>
#include <SPIFlash.h> //get it here: https://www.github.com/lowpowerlab/spiflash

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE *************
//*********************************************************************************************
#define SENDERNODEID  1    //must be unique for each node on same network (range up to 254, 255 is used for broadcast)
#define NETWORKID     100  //the same on all nodes that talk to each other (range up to 255)
#define RCVR1NODEID   2
#define RCVR2NODEID   3
#define FREQUENCY     RF69_915MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
//*********************************************************************************************

#define LED           9 // Moteinos have LEDs on D9

#define SERIAL_BAUD   115200

boolean requestACK = false;

#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif

//------------------------------------------------------------------
void setup() {
  Serial.begin(SERIAL_BAUD);
  Serial.println("moteinoSender!");

  radio.initialize(FREQUENCY,SENDERNODEID,NETWORKID);
  radio.setHighPower(); //uncomment only for RFM69HW!
  radio.encrypt(ENCRYPTKEY);
  //radio.setFrequency(919000000); //set frequency to some custom frequency

//Auto Transmission Control - dials down transmit power to save battery (-100 is the noise floor, -90 is still pretty good)
//For indoor nodes that are pretty static and at pretty stable temperatures (like a MotionMote) -90dBm is quite safe
//For more variable nodes that can expect to move or experience larger temp drifts a lower margin like -70 to -80 would probably be better
//Always test your ATC mote in the edge cases in your own environment to ensure ATC will perform as you expect
#ifdef ENABLE_ATC
  radio.enableAutoPower(-70);
#endif

  char buff[50];
  sprintf(buff, "\nTransmitting at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);

#ifdef ENABLE_ATC
  Serial.println("RFM69_ATC Enabled (Auto Transmission Control)\n");
#endif
}

byte receiver = 0;
char cmd[1];
//------------------------------------------------------------------
void loop() {
  //process any serial input
  if (Serial.available() > 0)
  {
    char input = Serial.read();
    Serial.print("received: "); Serial.println(input);

    if (input >= 48 && input <= 57) //[0,9]
      cmd[0] = input;

    if (input == 'a') //send to receiver 1
      receiver = RCVR1NODEID;
    if (input == 'b') //send to receiver 2
      receiver = RCVR2NODEID;
    
    if ((cmd[0] != null) && (receiver > 0))
    {
      Serial.print("sending: "); Serial.print(cmd[0]); Serial.print(" to: "); Serial.println(receiver);      //sendToReceiver(receiver, blinkCount);
      radio.send(receiver, cmd, 1);
      Blink(LED,3);
      receiver = 0;
    }     
  }
}

//------------------------------------------------------------------
void Blink(byte PIN, byte blinkCount) {
  pinMode(PIN, OUTPUT);
  for (byte i = 0; i < blinkCount; i++) {
    digitalWrite(PIN,HIGH);
    delay(1000);
    digitalWrite(PIN,LOW);
    delay(1000);
  }
}
