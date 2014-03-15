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
*/


#include <Arduino.h>
#include <ZMCameraLib.h>
#include <SoftwareSerial.h>
#include <SD.h>
#include "MemoryFree.h"

// *********************
// *** CONFIGURATION ***
// *********************

// *** SERIAL / CAMERA ***

#include "config.h"      // includes the file "config.h" that contains all parameters
                         // SERIAL_PORT, CS_PIN, SS_PIN, SERIAL_BAUDRATE...

#define ZM_CAMERA_ID  0x00  // the ID of your camera, if unknown try 0xFF or 0x00

#define SD_JPEG_FILENAME  "testc429.jpg"


// ***************
// *** CLASSES ***
// ***************

File myFile;
ZMCamera cam(&SERIAL_PORT);


// ******************
// *** MAIN SETUP ***
// ******************

void setup() {
  // initialize Serial for output
  Serial.begin(9600);
  
  // you should keep freeMemory here because RAM is an issue in this sketch (comm buffer with camera + memory needed for SD)
  Serial.print("freemem=");
  Serial.println(freeMemory());  // just for checking


  // SEQUENCE TO INITIALIZE THE SD CARD
  // AND CREATE THE OUTPUT FILE
  Serial.print("Initializing SD card...");

  pinMode(SS_PIN, OUTPUT);    // necessary (it seems ?) for the SD lib to work properly (dunno why)
  if (!SD.begin(CS_PIN)) {    // begin the SD lib using CS_PIN (see config.h for the value)
    Serial.println("initialization failed!");
    return;  // no need to continue beyond that point, sorry !
  }
  Serial.println("initialization done.");

  // if the file already exists, erase it !
  if (SD.exists(SD_JPEG_FILENAME)) {
    Serial.println("file already exists, erasing...");
    SD.remove(SD_JPEG_FILENAME);
  }

  // open the file for writing
  myFile = SD.open(SD_JPEG_FILENAME, FILE_WRITE);
  if (!myFile) {
    Serial.print("error opening ");
    Serial.print(SD_JPEG_FILENAME);
    return;  // no need to continue beyond that point, sorry !
  }


  // SEQUENCE TO INITIALIZE THE CAMERA LIB
  // ASK FOR A CAPTURE, AND STORE THE RESULT VIA THE SD
  SERIAL_PORT.begin(SERIAL_BAUDRATE);
  cam.setDebug(true);
  cam.setID(ZM_CAMERA_ID);
  
  // just checking if the cam is answering
  // she will provide a string indicating its version
  Serial.println("\n\n*** HELLO CAM");
  if (cam.helloCam() <= 0) {
    return;  // no need to continue beyond that point, sorry !
  }

  // change the compression ratio of the camera (JPEG)
  Serial.println("\n\n*** CHANGE COMPRESSION RATIO");  
  if (cam.changeCompressionRatio(100))
    Serial.println("OK");
  else
    Serial.println("NOT OK");


  // ASK FOR A CAPTURE BY THE CAMERA
  // all results will be stored on the camera, and you'll have to query for "packages"
  // (smaller bits of data that you have to aggregate afterwards to reconstitute the JPEG file
  Serial.println("\n\n*** CAPTURE PICTURE");  
  if (cam.captureImage(ZM_PICTURE_SIZE_1280X1024, 64))
    Serial.println("OK");
  else
    Serial.println("NOT OK");
  Serial.print("snapshot size: ");
  Serial.println(cam.getSnapshotSize());
  Serial.print("package count: ");
  uint16_t t_count = cam.getPackageCount();
  Serial.println(t_count);

  // ASK FOR THE PACKAGES AND REBUILD THE JPEG FILE PACKAGE BY PACKAGE
  // you're lucky, I've done a function that does everything ^^
  Serial.println("\n\n*** GET ALL PACKAGES");
  cam.setDebug(false);    // cause there is a LOT of transfers during this sequence
  uint16_t t_filesize = cam.processAllPackagesToFile(&myFile);  // see ZMCameraLib/ZMCameraLib.cpp

  // if the result size is null it means there has been an error in the process
  if (t_filesize == 0) {
    Serial.print("error code: ");
    Serial.println(cam.getErrorCode());  // see ZMCameraLib/ZMCameraLib.h
  } else {
    // outputs the size of the data received
    Serial.print("\n received ");
    Serial.print(t_filesize);
    // to be compared to the size of the data expected
    Serial.print(", orig snapshot size: ");
    Serial.println(cam.getSnapshotSize());
  }
  
  myFile.close();
}

// *****************
// *** MAIN LOOP ***
// *****************

void loop() {
  // nothing here...
}

