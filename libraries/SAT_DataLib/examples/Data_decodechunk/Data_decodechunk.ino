/*
********************************************************************

Copyright 2014, Jean-François Omhover (jf.omhover@gmail.com, twitter @jfomhover)

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

    Description :  decoding "packets" of data
                   for the purpose of this sketch, example packets are taken in the other example sketches
                   (Data_logpacket, Data_userpacket, Data_...) to demonstrate the use of the decoding scheme
                   Also see Data_decodefile for decoding binary packets stores on a SD file (via SAT_AppStorageEMUSD)
                   (see https://github.com/jfomhover/ArduSat-utils)
    Last Changed : Mar. 3, 2014

********************************************************************
*/

#include <SAT_DataLib.h>


DataDecoder decoder;	// class used for decoding packets


// ***********************
// *** PACKET EXAMPLES ***
// ***********************

// Output taken from the Data_buildchunk.ino example sketch
byte chunkpacket[11] = {
		0x23, 0x31, 0x00, 0xF2, 0x07, 0x00, 0x00, 0x19, 0x00, 0x40, 0x01
};


// Output taken from the Data_userdefinedblock.ino
byte chunkudblockpacket[17] = {
		0x23, 0x01, 0x60, 0xB2, 0x5F, 0x01, 0x00, 0x03
		0x04, 0x08, 0x15, 0x16, 0x0D, 0x53, 0x54, 0x4F
		0x50 
};

// Output taken from the EMUSD_send_chunk.ino
byte chunktmppacket[15] = {
		0x23, 0xF1, 0x00, 0xA3, 0x15, 0x00, 0x00, 0x04,
		0x00, 0x08, 0x00, 0x0F, 0x00, 0x10, 0x00 
};

// Output of the Data_userpacket.ino example sketch
byte userpacket[25] = {
		0x55, 0x19, 0x0F, 0x56, 0x0E, 0x49, 0x40, 0x05,
		0x2A, 0x00, 0x07, 0x47, 0xBC, 0xDE, 0x02, 0x03
		0xAB, 0xCD, 0xEF, 0x01, 0x0D, 0x53, 0x54, 0x4F
		0x50 
};

// Output of the Data_logpacket.ino example sketch
byte logpacket[19] = {
		0x4C, 0x13, 0x6C, 0x6F, 0x67, 0x20, 0x61, 0x6E,
		0x79, 0x74, 0x68, 0x69, 0x6E, 0x67, 0x20, 0x68,
		0x65, 0x72, 0x65
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
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.print("packet[]= ");           // display the packet
  dumphex(chunkpacket, 11);
  decoder.parseBuffer(chunkpacket);     // decodes one packet
  Serial.print('\n');
  
  Serial.print("packet[]= ");           // display the packet
  dumphex(chunkudblockpacket, 17);
  decoder.parseBuffer(chunkudblockpacket);     // decodes one packet
  Serial.print('\n');

  Serial.print("packet[]= ");           // display the packet
  dumphex(chunktmppacket, 15);
  decoder.parseBuffer(chunktmppacket);     // decodes one packet
  Serial.print('\n');

  Serial.print("packet[]= ");           // display the packet
  dumphex(userpacket, 25);
  decoder.parseBuffer(userpacket);      // decodes one packet
  Serial.print('\n');

  Serial.print("packet[]= ");           // display the packet
  dumphex(logpacket, 19);
  decoder.parseBuffer(logpacket);       // decodes one packet
  Serial.print('\n');
}


void loop() {
  // nothing done here...
}