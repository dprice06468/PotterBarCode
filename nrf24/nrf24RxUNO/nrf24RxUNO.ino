#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

// Set up nRF24L01 radio on SPI bus plus pins 9 & 53 (for Mega) 

RF24 radio(9,53);

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipe = 0xF0F0F0F0E1LL;

// define the buffer to hold the message
char msg[10];

void setup(void)
{
  Serial.begin(57600);
  printf_begin();
  printf("\n\rRF24TxDataMega\n\r");

  // Setup and configure rf radio
  radio.begin();

  // optionally, increase the delay between retries & # of retries
//  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to improve reliability
  //radio.setPayloadSize(8);

  // Open pipes to other nodes for communication
  radio.openWritingPipe(pipe);

  // Dump the configuration of the rf unit for debugging
  radio.printDetails();
}

void loop(void) {
}

void serialEvent() {
  memset(msg, ' ', sizeof(msg));
  Serial.readBytesUntil('\n', msg, sizeof(msg));
  radio.write(msg, sizeof(msg));
}

