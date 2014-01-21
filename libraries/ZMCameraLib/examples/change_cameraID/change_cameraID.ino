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

#define NEW_ZMCAMERA_ID  0x01  // the NEW ID you'd like the camera to have

#define NEW_BAUDRATE     9600
#define NEW_ZM_BAUDRATE  ZM_BAUDRATE_9600  // USE ONE OF THE CONSTANTS BELOW ACCORDING TO THE DESIRED BAUDRATE
/* AVAILABLE CONSTANTS (see ZMCameraLib/ZMCameraLib.h)
#define ZM_BAUDRATE_9600    '0'
#define ZM_BAUDRATE_19200   '1'
#define ZM_BAUDRATE_38400   '2'
#define ZM_BAUDRATE_57600   '3'
#define ZM_BAUDRATE_115200  '4'
#define ZM_BAUDRATE_2400    '5'
#define ZM_BAUDRATE_14400   '6'
*/

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
  
  // the following line initializes the library with camera ID 0xFF which calls for every camera
  // you can comment this line, since the library is already usingID 0xFF when no ID is specified (see constructor)
  cam.setID(0xFF);
  
  Serial.println("\n\n*** HELLO CAM");
  cam.helloCam();    // say hello to the camera... that will answer with it's version string

  Serial.print("\n\n*** CHANGE ID TO");
  Serial.println(NEW_ZMCAMERA_ID);
  if (cam.changeID(NEW_ZMCAMERA_ID))
    Serial.println("OK");
  else
    Serial.println("NOT OK");

  Serial.println("\n\n*** HELLO AGAIN (just checking)");
  cam.helloCam();    // say hello AGAIN to the camera to verify the baudrate has been adjusted
}

// *****************
// *** MAIN LOOP ***
// *****************

void loop() {
  // nothing here...
}

