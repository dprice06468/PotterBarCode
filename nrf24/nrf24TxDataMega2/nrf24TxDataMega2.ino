#include <SPI.h>
//#include "nRF24L01.h"
#include "RF24.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 53 (for Mega) 
RF24 radio(9,53);

// Radio pipe addresses for the 2 nodes to communicate.
byte pipe[][6] = {"1Node"};

// define the buffer to hold the message
char msg[10];

void setup(void)
{
  Serial.begin(115200);
  delay(1000);
  Serial.println(F("RF24/Simple Transmit data Test"));
  Serial.println(F("Questions: terry@yourduino.com"));
  //dataTransmitted = 100; // Arbitrary known data to transmit. Change it to test...
  radio.begin();  // Start up the physical nRF24L01 Radio
  radio.setChannel(108);  // Above most Wifi Channels
  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices. RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MIN);
  //  myRadio.setPALevel(RF24_PA_MAX);  // Uncomment for more power

  radio.openWritingPipe(pipe[0]); // Use the first entry in array 'addresses' (Only 1 right now)
  delay(1000);
}

void loop(void) {
}

void serialEvent() {
  memset(msg, ' ', sizeof(msg));
  Serial.readBytesUntil('\n', msg, sizeof(msg));
  Serial.println(msg);
  radio.write(msg, sizeof(msg));
  Serial.print(F("Data Transmitted = "));
  Serial.print(msg);
  Serial.println(F(" No Acknowledge expected"));
  delay(500);
}

