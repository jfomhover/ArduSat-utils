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

    Description :  example sketch for the HSEncodeHandler,
                   compresses data stored in an array in memory and show it on Serial
    Last Changed : Feb. 2, 2014

********************************************************************
*/

#include <Arduino.h>
#include <HeatShrinkLib.h>
#include "MemoryFree.h"
#include "dataset.h"

void dumphex(byte * data, int len);

HSEncodeHandler handler;

void setup() {
  Serial.begin(9600);

  Serial.print("freeram=");
  Serial.println(freeMemory());
  Serial.print("sizeof(encoder)=");
  Serial.println(sizeof(handler)); 
  
  handler.reset();						// reset the internal structure
  handler.setupOutputStream(dumphex);	// sets the function that will be called by the handler when output data is available
    	  	  	  	  	  	  	  	  	// this function must be defined as : void function(byte *, int)

  handler.push(TEST_DATA_PTR, TEST_DATA_SIZE);

  // you can also choose to push data chunk by chunk, like in the following
  /*
  for (int i=0; i<TEST_DATA_SIZE; i+=CHUNK_SIZE) {
    int t_size = CHUNK_SIZE;
    if ((TEST_DATA_SIZE - i) < t_size)
      t_size = (TEST_DATA_SIZE - i);
      for (int k=0; k<t_size; k++)
        buffer[k] = TEST_DATA_PTR[i+k];
    handler.push(buffer, t_size);
  }
  */

  handler.flush();			// flush output, force encoding
  handler.displayStats();	// display the stats of the handler (data received, data sent)
}


void loop() {
};

// just a function for demonstrating the functions
// this function outputs a binary message in hexadecimal on serial
int index = 0;

void dumphex(byte * data, int len) {
//  return;
  for (int i=0; i<len; i++) {
      Serial.print("0x");
      if (data[i]<0x10) {Serial.print('0');}
      Serial.print(data[i],HEX);
      if (((index+1) % 8) == 0)
        Serial.print(",\n");
      else
        Serial.print(", ");
      index++;
    }
}
