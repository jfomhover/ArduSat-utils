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

// structured packet of data, user defined
// it is formatted in order to be decoded after reception (see https://github.com/jfomhover/ArduSat-utils for syntax)
struct _dataPacket {
  char header;	// should contain PACKET_HEADER_USERPACKET ('U'), see datalib_defs.h
  uint8_t len;	// should indicate the total length of the packet, for instance use sizeof(struct _dataPacket)

  // then, each user defined data is structured by a TYPE/VALUE sequence
  uint8_t type1;	// TYPE : for a float, use DATATYPE_UNIT_FLOAT (12)
  float fval;		// VALUE : fill with the value you'd like

  uint8_t type2;	// TYPE : for an int (2 bytes, "short int"), use DATATYPE_UNIT_INTEGER
  int ival;			// VALUE : fill with the int value

  uint8_t type3;			// TYPE : for an unsigned long int, use DATATYPE_UNIT_ULONGINT
  unsigned long int lval;	// VALUE...

  uint8_t type4;	// TYPE : for an hexadecimal sequence of 4 bytes, use DATATYPE_UNIT_HEX4
  byte hex4val[4];	// VALUE : here, the array of bytes

  uint8_t type5;	// TYPE : for 4 chars (no more, no less), use DATATYPE_UNIT_STR
  char charval[4];	// VALUE : here, the array of chars
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
  // STEP 1 : use the header ('U') for the packet to be decoded by SAT_DataLib
  data.header = PACKET_HEADER_USERPACKET;

  // STEP 2 : indicate the size of the packet
  data.len = PACKET_SIZE;

  // STEP 3 : fill the structure with the raw values you want to capture
  // see the structure above for explanation on type1...type5
  data.type1 = DATATYPE_UNIT_FLOAT;
  data.fval = 3.1415;

  data.type2 = DATATYPE_UNIT_INTEGER;
  data.ival = 42;

  data.type3 = DATATYPE_UNIT_LONGINT;
  data.lval = 48151623;

  data.type4 = DATATYPE_UNIT_HEX4;
  data.hex4val[0] = 0xAB;
  data.hex4val[1] = 0xCD;
  data.hex4val[2] = 0xEF;
  data.hex4val[3] = 0x01;

  data.type5 = DATATYPE_UNIT_STR;
  data.charval[0] = 'S';
  data.charval[1] = 'T';
  data.charval[2] = 'O';
  data.charval[3] = 'P';
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

  // should output : 55 19 0C 56 0E 49 40 06 2A 00 04 47 BC DE 02 00 AB CD EF 01 0D 53 54 4F 50

  delay(5000);
}