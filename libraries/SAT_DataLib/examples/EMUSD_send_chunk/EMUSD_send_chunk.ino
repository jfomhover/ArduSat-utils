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

    Description :  example use of SAT_AppStorage emulator (SD version)
                   sends a "chunk" formatted as specified in the SAT_DataLib library
    Last Changed : Feb. 1, 2014

********************************************************************
*/

#include <SD.h>
#include <Wire.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>
#include <nanosat_message.h>
#include <SAT_AppStorageEMUSD.h>
#include <SAT_DataLib.h>

SAT_AppStorageEMUSD store;    // instance of the class


// ***********************
// *** DATA STRUCTURES ***
// ***********************

// structured packet of data, easy to decode after reception (see https://github.com/jfomhover/ArduSat-utils for syntax)
// for this example sketch we use a "chunk" packet, and we'll send it through SAT_AppStorageEMUSD
struct _dataPacket {
char header;          // should be set to PACKET_HEADER_CHUNK ('#')

uint16_t datatypes;   // should indicate all the datatypes by union of the values
                      // in this sketch data.dataypes = DATATYPE_MS | DATATYPE_TMP1 | DATATYPE_TMP2 | DATATYPE_TMP3 | DATATYPE_TMP4

long int ms;          // corresponding to datatype DATATYPE_MS

int16_t tmp1;         // corresponding to datatype DATATYPE_TMP1
int16_t tmp2;         // corresponding to datatype DATATYPE_TMP2
int16_t tmp3;         // corresponding to datatype DATATYPE_TMP3
int16_t tmp4;         // corresponding to datatype DATATYPE_TMP4
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
  data.datatypes = DATATYPE_MS | DATATYPE_SAT_TMP1 | DATATYPE_SAT_TMP2 | DATATYPE_SAT_TMP3 | DATATYPE_SAT_TMP4;

  // STEP 3 : fill the structure with the raw values you want to capture
  data.ms = millis();
  data.tmp1 = 4;
  data.tmp2 = 8;
  data.tmp3 = 15;
  data.tmp4 = 16;
};


// ********************
// *** SETUP + LOOP ***
// ********************

void setup() {
  // initialization of the emulator (SD configuration)
  Serial.begin(9600);

  store.init( true,               // debug mode : outputs strings on Serial
              4,                  // chip select : pin of the chip select on your arduino config (uno ethernet = 4)
              false,              // append : true if data sent is appended at the end of the existing file)
              "datachk.bin" );    // filename : name of the file writen on SD
}


void loop() {
  resetPacket();  // zeroes the values of the data structure (you never know...)
  pullValues();   // fills the data structure with values (IRL, from sensors)
  
  byte * t_bytePtr = (byte *)&data;        // gets a byte* pointer from the pointer to the data structure

  store.send(t_bytePtr, 0, PACKET_SIZE);   // sends data into the communication file and queue for transfer
                                           // WARNING : introduces a 100ms delay

  delay(5000);
}

