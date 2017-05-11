#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10 (for uno or 53 for Mega) 
RF24 radio(9,10);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipe = 0xF0F0F0F0E1LL;

// define the buffer to hold the message
char msg[10];

void setup(void)
{
  Serial.begin(57600);
  printf_begin();
  printf("RF24RxDataUNO\n\r");

  // Setup and configure rf radio
  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to improve reliability
  //radio.setPayloadSize(8);

  // Open pipes to other nodes for communication
  radio.openReadingPipe(1, pipe);

  // Start listening
  radio.startListening();

  Serial.println("I'm listening");
}

void loop(void) {
  if (radio.available()) {
    Serial.println("available");
    memset(msg, ' ', sizeof(msg));
    radio.read(msg, sizeof(msg));
    Serial.println(msg);
  }
}

