#include <RFM69.h>    //get it here: https://www.github.com/lowpowerlab/rfm69
#include <RFM69_ATC.h>//get it here: https://www.github.com/lowpowerlab/rfm69
#include <SPI.h>      //comes with Arduino IDE (www.arduino.cc)
#include <SPIFlash.h> //get it here: https://www.github.com/lowpowerlab/spiflash

//*********************************************************************************************
//************ IMPORTANT SETTINGS - YOU MUST CHANGE/CONFIGURE TO FIT YOUR HARDWARE *************
//*********************************************************************************************
#define NODEID        2    //unique for each node on same network
#define NETWORKID     100  //the same on all nodes that talk to each other
#define FREQUENCY     RF69_915MHZ
#define ENCRYPTKEY    "sampleEncryptKey" //exactly the same 16 characters/bytes on all nodes!
#define IS_RFM69HW    //uncomment only for RFM69HW! Leave out if you have RFM69W!
#define ENABLE_ATC    //comment out this line to disable AUTO TRANSMISSION CONTROL
//*********************************************************************************************

#define SERIAL_BAUD   115200

#define LED           9 // Moteinos have LEDs on D9

#ifdef ENABLE_ATC
  RFM69_ATC radio;
#else
  RFM69 radio;
#endif

//SPIFlash flash(FLASH_SS, 0xEF30); //EF30 for 4mbit  Windbond chip (W25X40CL)

//------------------------------------------------------------------
void setup() {
  Serial.begin(SERIAL_BAUD);
  delay(10);
  radio.initialize(FREQUENCY,NODEID,NETWORKID);
  radio.setHighPower(); //only for RFM69HW!
  radio.encrypt(ENCRYPTKEY);
  radio.promiscuous(false);
  //radio.setFrequency(919000000); //set frequency to some custom frequency
  char buff[50];
  sprintf(buff, "\nListening at %d Mhz...", FREQUENCY==RF69_433MHZ ? 433 : FREQUENCY==RF69_868MHZ ? 868 : 915);
  Serial.println(buff);
    
#ifdef ENABLE_ATC
  Serial.println("RFM69_ATC Enabled (Auto Transmission Control)");
#endif
}

//------------------------------------------------------------------
void loop() {
  //process any serial input
  if (Serial.available() > 0)
  {
    char cmd = Serial.read();;  //to hold char data received

    Serial.print("received: "); Serial.println(cmd);
    byte i = cmd-48;
    Blink(LED,i);
  }
  
  if (radio.receiveDone())
  {
    char cmd = (char)radio.DATA[0];

    Serial.print("received: "); Serial.println(cmd);

    Blink(LED,cmd-48);
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
