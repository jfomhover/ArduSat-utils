/*
********************************************************************

Copyright 2014, Jean-François Omhover (jf.omhover@gmail.com) and Nanosatisfi, Inc. (www.nanosatisfi.com)
idea suggested by Pablo Pita, <pablo.pita@gmail.com>
based on the work by Scott Vokes <scott.vokes@atomicobject.com>

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

    Description :  example sketch for the HSDecodeHandler,
                   uncompressed data stored in an array in memory and show it on Serial
    Last Changed : Feb. 2, 2014

********************************************************************
*/

#include <Arduino.h>
#include <HeatShrinkLib.h>
#include "MemoryFree.h"
#include "dataset.h"

// ******************
// *** MAIN SETUP ***
// ******************
void dumphex(byte * data, int len);

HSDecodeHandler handler;

void setup() {
  Serial.begin(9600);

  handler.reset();	// reset the internal structure
  handler.setupOutputStream(dumphex);	// sets the function that will be called by the handler when output data is available
  	  	  	  	  	  	  	  	  	  	// this function must be defined as : void function(byte *, int)

  handler.push(ENCODE_DATA_PTR, ENCODE_DATA_SIZE);		// push data for decoding
  handler.purgeDecompressOutput();						// flush data, forcing output
  
  Serial.print("freeram=");
  Serial.println(freeMemory());
  
  handler.displayStats();								// display the stats of the handler (data received, data sent)
}


void loop() {
};

// just a function for demonstrating the functions
// this function outputs a binary message in hexadecimal on serial
int index = 0;

void dumphex(byte * data, int len) {
  for (int i=0; i<len; i++) {
      Serial.print("0x");
      if (data[i]<0x10) {Serial.print('0');}
      Serial.print(data[i],HEX);
      if (((index+1) % 8) == 0)
        Serial.print(",\n");
      else
        Serial.print(" ");
      index++;
    }
}
