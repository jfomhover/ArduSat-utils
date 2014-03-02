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

    Description :  sketch that demonstrates how to use chunk packets with the "USERDEFINED" blocks
                   https://github.com/jfomhover/ArduSat-utils
    Last Changed : Mar. 3, 2014

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
  char header;                 // should be set to PACKET_HEADER_CHUNK ('#')

  uint16_t datatypes;          // should indicate all the datatypes by union of the values
                               // in this sketch data.dataypes = DATATYPE_MS | DATATYPE_USERDEFINED1 | DATATYPE_USERDEFINED2

  long int ms;                 // corresponding to datatype DATATYPE_MS
  byte userdefinedblock[5];    // block of 5 bytes corresponding to datatype DATATYPE_USERDEFINED1
  byte userdefinedblock2[5];   // block of 5 bytes corresponding to datatype DATATYPE_USERDEFINED2
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
  // STEP 1 : always use the header ('#') for the packet to be decoded by SAT_DataLib
  data.header = PACKET_HEADER_CHUNK;

  // STEP 2 : put all the datatypes you want to use in data.datatypes (make an OR of all DATATYPE_* constants you want)
  data.datatypes = DATATYPE_MS | DATATYPE_USERDEFINED1 | DATATYPE_USERDEFINED2;

  // STEP 3 : fill the structure with the raw values you want to capture
  // NOTE : in this sketch, only MS and USERDEFINED1 are "pulled"
  data.ms = millis();

  // you can fill the remaining 4 bytes one by one like below
  // OPTION HEX
  data.userdefinedblock[0] = DATATYPE_UNIT_HEX32; // using this as first byte will display the other 4 bytes in binary format on the decoder
  data.userdefinedblock[1] = 0x04;
  data.userdefinedblock[2] = 0x08;
  data.userdefinedblock[3] = 0x15;
  data.userdefinedblock[4] = 0x16;

  // OPTION "STRING"
  data.userdefinedblock2[0] = DATATYPE_UNIT_STR; // using this as first byte will display the other 4 bytes as chars
  data.userdefinedblock2[1] = 'S';
  data.userdefinedblock2[2] = 'T';
  data.userdefinedblock2[3] = 'O';
  data.userdefinedblock2[4] = 'P';

  // or you can fill with a value taken from somewhere...
//  data.userdefinedblock[0] = DATATYPE_UNIT_UINT32; // using this as first byte will display the other 4 bytes as an unsigned long int
//  unsigned long int whatever = 481516;
//  memcpy((void*)(data.userdefinedblock), (void*)&whatever, sizeof(whatever));  // copy the bytes of the unsigned long int to the block

  // see datalib_defs.h for the possible types

  // THAT'S IT ^^
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

  dumphex((byte*)&data, PACKET_SIZE);		// displays the binary content of the data structure, for pedagogical purpose here ^^

  // should output : 23 01 60 00 00 00 00 03 04 08 15 16 0D 53 54 4F 50 

  delay(5000);
}