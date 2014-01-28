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

// *********************
// *** CONFIGURATION ***
// *********************

// *** SERIAL / CAMERA ***

#include "config.h"      // includes the file "config.h" that contains all parameters
                         // SERIAL_PORT, CS_PIN, SS_PIN, SERIAL_BAUDRATE...

#define ZM_CAMERA_ID  0x01  // the ID of your camera, if unknown try 0xFF

// ***************
// *** CLASSES ***
// ***************

ZMCamera cam(&SERIAL_PORT);


// ******************
// *** MAIN SETUP ***
// ******************

void setup() {
  // initialize Serial for output
  Serial.begin(9600);
  
  // initialize the serial connection toward the Camera
  SERIAL_PORT.begin(SERIAL_BAUDRATE);
  
  // set the debug mode to true (to display binary exchanges with the Camera on Serial)
  cam.setDebug(true);
  
  // configures the id of the camera you're talking to
  cam.setID(ZM_CAMERA_ID);  
  
  Serial.println("\n\n*** HELLO CAM");
  cam.helloCam();    // say hello to the camera... that will answer with it's version string
}

// *****************
// *** MAIN LOOP ***
// *****************

void loop() {
  // nothing here...
}

