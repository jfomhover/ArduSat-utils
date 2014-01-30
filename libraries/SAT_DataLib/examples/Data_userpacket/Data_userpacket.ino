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

    Description :  sketch that demonstrates how to use the "USERDEFINED" blocks from SAT_DataLib
                   https://github.com/jfomhover/ArduSat-utils
    Last Changed : Jan. 27, 2014

********************************************************************
*/


#include <Arduino.h>
#include <Wire.h>              // NOTE: we don't call any real sensor in this sketch, so this line is unneccesary
#include <EEPROM.h>
#include <OnboardCommLayer.h>  // NOTE: we don't call any real sensor in this sketch, so this line is unneccesary
#include <SAT_DataLib.h>
#include <SD.h>
#include <SAT_AppStorageEMUSD.h>  // experiment data channel (on SD, see library at https://github.com/jfomhover/ArduSat-utils)


// *************************
// *** EXPERIMENT CONFIG ***
// *************************

#define PULL_DELAY 5000        // data is pooled every PULL_DELAY milliseconds

// NOTE : configuration of the debug mode for writing on SD card
#define CS_PIN      4    // 4 for Arduino Ethernet
#define SD_FILENAME  "udpacket.bin"


// *********************
// *** SDK INSTANCES ***
// *********************

SAT_AppStorageEMUSD store;    // experiment data channel (via SD, see library at https://github.com/jfomhover/ArduSat-utils )


// ***********************
// *** DATA STRUCTURES ***
// ***********************

// structured packet of data, easy to decode after reception (see https://github.com/jfomhover/ArduSat-utils for syntax)
// for this example sketch we keep only DATATYPE_MS and DATATYPE_USERDEFINED1
struct _dataPacket {
  char header;
  uint8_t len;
  uint8_t type1;
  float fval;
  uint8_t type2;
  int ival;
  uint8_t type3;
  unsigned long int lval;
  uint8_t type4;
  byte hex4val[4];
  uint8_t type5;
  char charval[4];
} data;

#define PACKET_SIZE sizeof(struct _dataPacket)

// erasing all values in the data chunk
void resetChunk() {
  byte * t_ptr = (byte*)&data;
  for (int i=0; i<PACKET_SIZE; i++)
    t_ptr[i] = 0x00;
}


// ************************
// *** SENSOR FUNCTIONS ***
// ************************

// SETUP OF THE SENSORS NEEDED (constructors mainly)
void setupSensors() {
  // NOTE : add the setup functions you'd like, in this sketch, none is needed
}


// pulling values from the sensors and filling the data structure
void pullValues() {
  // STEP 1 : use the header ('U') for the packet to be decoded by SAT_DataLib
  data.header = PACKET_HEADER_USERPACKET;
  
  // STEP 2 : 
  data.len = PACKET_SIZE;
  
  // STEP 3 : fill the structure with the raw values you want to capture
  data.type1 = DATATYPE_USERDEFINED_FLOAT;
  data.fval = 3.1415;
  
  data.type2 = DATATYPE_USERDEFINED_INTEGER;
  data.ival = 42;
  
  data.type3 = DATATYPE_USERDEFINED_LONGINT;
  data.lval = 48151623;
  
  data.type4 = DATATYPE_USERDEFINED_HEX4;
  data.hex4val[0] = 0xAB;
  data.hex4val[1] = 0xCD;
  data.hex4val[2] = 0xEF;
  data.hex4val[3] = 0x01;
  
  // OPTION "STRING"
  data.type5 = DATATYPE_USERDEFINED_STR;
  data.charval[0] = 'S';
  data.charval[1] = 'T';
  data.charval[2] = 'O';
  data.charval[3] = 'P';
};


// *********************************************************************
// NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE NOTE
// *********************************************************************
// The remaining setup() and loop() just calls for the functions above
// It "pulls" the sensors regularly and sends the data chunk to SAT_AppStorageEMUSD


// ******************
// *** MAIN SETUP ***
// ******************

byte buffer[40];

void setup() {
  Serial.begin(9600);

  OBCL.begin();             // setups the communication via I2C (sensors + AppStorage)

  store.init( true, CS_PIN, false, SD_FILENAME ); // debug : outputing verbose lines on Serial + write result on SD file
  
  setupSensors();
}


// *****************
// *** MAIN LOOP ***
// *****************
 
signed long int previousMs = 0;
signed long int nextMs = 0;
int dataSent = 0;

void loop() {
  resetChunk();      // zeroes the data structure

  nextMs = PULL_DELAY-(millis()-previousMs);  // "(not so) intelligent delay" : just the ms needed to have a regular timing, takes into account the delay of all the functions in the loop
  previousMs += PULL_DELAY;
  
  if (nextMs > 0)
    delay(nextMs); //wait for next pull
  else
    previousMs += PULL_DELAY; // we missed a beat, skip one to get back on regular track

  pullValues();   // pull the values needed

  byte * t_ptr = (byte *)&data;
  store.send(t_ptr, 0, PACKET_SIZE);   // sends data into the communication file and queue for transfer
                                       // WARNING : introduces a 100ms delay

  dataSent += PACKET_SIZE;
  
/*
  if ((dataSent + PACKET_SIZE) > 10240) {
    // TODO : close experiments, clean mode ?
    // my understanding is : that will be done automatically when we reach the 10kb limit
  }
*/
}

