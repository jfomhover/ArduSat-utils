/*
    Author :       Jean-Francois Omhover (jf.omhover@gmail.com, twitter:@jfomhover)
    URL :          https://github.com/jfomhover/ArduSat-utils
    
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <Arduino.h>
#include <ZMCameraLib.h>


// *********************
// *** CONFIGURATION ***
// *********************

// *** SERIAL / CAMERA ***

#include "config.h"      // includes the file "config.h" that contains all parameters
                         // SERIAL_PORT, CS_PIN, SS_PIN, SERIAL_BAUDRATE...

#define ZM_CAMERA_ID  0x01  // the ID of your camera, if unknown try 0xFF

#define SD_JPEG_FILENAME  "testCL3.jpg"


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
  if (cam.captureImage(ZM_PICTURE_SIZE_640X480, 64))
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

