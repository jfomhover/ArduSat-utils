/*
********************************************************************

Copyright 2014, Jean-François Omhover (jf.omhover@gmail.com)

********************************************************************
Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
********************************************************************

    Description :  sketch that demonstrates how to use the user defined packets from SAT_DataLib
                   https://github.com/jfomhover/ArduSat-utils
    Last Changed : Feb. 1, 2014

********************************************************************
*/


#include <Arduino.h>
#include <SAT_DataLib.h>


// ***********************
// *** DATA STRUCTURES ***
// ***********************

// structured packet of data, easy to decode after reception (see https://github.com/jfomhover/ArduSat-utils for syntax)
// for this example sketch we use a Log packet
struct _dataPacket {
  char header;	// should be set to PACKET_HEADER_LOGPACKET ('L')
  uint8_t len;	// should be set to the REAL total length of the packet
  	  	  	    // (for instance, PACKET_SIZE_LOGHEADER + strlen(data.string));

  char string[64];	// buffer for the string we'd like to send
  	  	  	  	  	// we won't send 64 chars, but only the length really used
} data;

#define PACKET_SIZE sizeof(struct _dataPacket)

// erasing all values in the data chunk
void resetPacket() {
  byte * t_ptr = (byte*)&data;
  for (unsigned int i=0; i<PACKET_SIZE; i++)
    t_ptr[i] = 0x00;
}


// **********************
// *** PULLING VALUES ***
// **********************

// pulling values and filling the data structure
// IRL, these values would came from pulling sensors, or computing some parameters...
void pullValues() {
	  // STEP 1 : use the header ('L') for the packet to be decoded by SAT_DataLib
	  data.header = PACKET_HEADER_LOGPACKET;

	  // STEP 2 : fill the structure with the chars you'd like to send
	  sprintf(data.string, "log anything here");

	  // STEP 3 : align the data.len with the REAL total length of the packet
	  // if you set it to sizeof(struct _dataPacket) [like in other sketches]
	  // you will send more chars than needed because of the maximum size of the buffer (64)
	  // PACKET_SIZE_LOGHEADER = 2 (sizeof(char) for data.header + sizeof(uint8_t) for data.len)
	  data.len = PACKET_SIZE_LOGHEADER+strlen(data.string);
};


// *************
// *** DEBUG ***
// *************

// just a function for demonstrating the functions
// this function outputs a binary message in hexadecimal on serial
void dumphex(byte * data, int len) {
  for (int i=0; i<len; i++) {
    if (data[i]<0x10) {Serial.print('0');}
      Serial.print(data[i],HEX);
      Serial.print(' ');
    }
    Serial.print('\n');
}

// ********************
// *** SETUP + LOOP ***
// ********************

void setup() {
  Serial.begin(9600);
}


void loop() {
  resetPacket();	// zeroes the data structure (you'd better clean things, you never know)
  pullValues();		// fills the data structure with values

  // the data is now ready to be sent !
  // for instance, use SAT_AppStorage.send((byte*)&data, 0, sizeof(struct _dataPacket));

  dumphex((byte*)&data, data.len);		// displays the binary content of the data structure, for pedagogical purpose here ^^

  // should output : 53 13 6C 6F 67 20 61 6E 79 74 68 69 6E 67 20 68 65 72 65

  delay(5000);
}
