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

    Description :  decodes a file on the SD card that containts packets of data
                   formatted as specified in the SAT_DataLib coding scheme
                   https://github.com/jfomhover/ArduSat-utils
                   Also see SAT_AppStorageEMUSD examples
    Last Changed : Feb. 1, 2014

********************************************************************
*/

#include <SD.h>
#include <SAT_DataLib.h>

const int chipSelect = 4;              // pin used as ChipSelect (see SD library)
char filename[] = "fullpack.bin";      // name of the file on SD
byte buffer[64];                       // a buffer for the decoder to run, I recommend at least 64 bytes of data


DataDecoder decoder;


void setup() {
  // put your setup code here, to run once:
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

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(filename);

  // if the file is available, write to it:
  if (dataFile) {
    decoder.parseFile(dataFile, buffer, 40);
    dataFile.close();
  }
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datachk.bin");
  }

}


void loop() {
  // put your main code here, to run repeatedly:
}
