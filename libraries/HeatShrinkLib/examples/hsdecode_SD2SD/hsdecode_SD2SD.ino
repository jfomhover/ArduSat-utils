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
                   reads a file on SD, uncompressed the data, and stores the output on SD
    Last Changed : Feb. 2, 2014

********************************************************************
*/

#include <Arduino.h>
#include <HeatShrinkLib.h>
#include "MemoryFree.h"
#include <SD.h>

const int chipSelect = 4;
// #define DEBUG_MODE	// uncomment for verbose output


#define INBUF_SIZE  32
#define OUTBUF_SIZE  64
#define INPUT_FILE  "devin.bin"
#define OUTPUT_FILE  "devout.bin"

File inFile;
File outFile;
byte in_buf[INBUF_SIZE];
int in_size = 0;
byte out_buf[OUTBUF_SIZE];
int out_size = 0;
  
void dumphex(byte * data, int len);

// ******************
// *** MAIN SETUP ***
// ******************
void my_send(byte * data, int len) {
#ifdef DEBUG_MODE
  dumphex(data,len);	// uncomment for debug
#endif
  outFile.write(data, len);
  outFile.flush();
}

HSDecodeHandler handler;

void setup() {
  Serial.begin(9600);

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  handler.reset();
  handler.setupOutputStream(my_send);

  if(SD.exists(OUTPUT_FILE))
    {
        SD.remove(OUTPUT_FILE);
        Serial.println("output file removed");
    }
    else
    {
        Serial.println("no output file to remove");
    }
  
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  inFile = SD.open(INPUT_FILE);
  if (inFile)
    Serial.println("inFile opened");
  
  outFile = SD.open(OUTPUT_FILE, FILE_WRITE);
  if (outFile)
    Serial.println("output file opened");
  
  // if the file is available, write to it:
  if (inFile && outFile) {
    while (inFile.available()) {
      in_size = 0;
      for (int i=0; i<INBUF_SIZE; i++)
        if (inFile.available()) {
          in_buf[in_size++] = inFile.read();
        } else {
          Serial.println("end of input.");
          break;
        }
      if (in_size > 0) {
        handler.push(in_buf, in_size);
      }
    }
    handler.purgeDecompressOutput();    
    inFile.close();
    outFile.close();
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening files");
  }
  Serial.println("finished.");

  handler.displayStats();
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
