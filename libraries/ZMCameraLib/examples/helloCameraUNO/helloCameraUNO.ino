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

Special for arduino UNO : use Serial (hardware) to establish a first contact
with a camera set up on 115200 (out of factory)

Debug output is displayed on SoftwareSerial(RX=7,TX=8).

********************************************************************
*/

 
#include <Arduino.h>
#include <ZMCameraLib.h>
#include <SoftwareSerial.h>
    
// *********************
// *** CONFIGURATION ***
// *********************

// *** SERIAL / CAMERA ***
#define SERIAL_SOFT_RX  7
#define SERIAL_SOFT_TX  8
#include <SoftwareSerial.h>
SoftwareSerial mySerial(SERIAL_SOFT_RX, SERIAL_SOFT_TX); // RX, TX

#define ZM_CAMERA_ID  0x01  // the ID of your camera, if unknown try 0xFF

// ***************
// *** CLASSES ***
// ***************

ZMCamera cam(&Serial);


// ******************
// *** MAIN SETUP ***
// ******************

void setup() {
  // initialize Serial for output
  Serial.begin(115200);
  
  // initialize the serial connection toward the Camera
  mySerial.begin(9600);
  
  // set the debug mode to true (to display binary exchanges with the Camera on Serial)
  cam.setDebugStream(&mySerial);
  cam.setDebug(true);
  
  // configures the id of the camera you're talking to
  cam.setID(ZM_CAMERA_ID);  
  
  mySerial.println("\n\n*** HELLO CAM");
  cam.helloCam();    // say hello to the camera... that will answer with it's version string
}

// *****************
// *** MAIN LOOP ***
// *****************

void loop() {
  // nothing here...
}

