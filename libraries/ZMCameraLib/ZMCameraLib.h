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

#ifndef _ZMCAMERALIB_H_
#define _ZMCAMERALIB_H_

#define ZM_BAUDRATE_9600    '0'
#define ZM_BAUDRATE_19200   '1'
#define ZM_BAUDRATE_38400   '2'
#define ZM_BAUDRATE_57600   '3'
#define ZM_BAUDRATE_115200  '4'
#define ZM_BAUDRATE_2400    '5'
#define ZM_BAUDRATE_14400   '6'

#define ZM_PICTURE_SIZE_160X128    '1'
#define ZM_PICTURE_SIZE_320X240    '2'
#define ZM_PICTURE_SIZE_640X480    '3'
#define ZM_PICTURE_SIZE_1280X1024  '4'

#define ZM_ERROR_NACKRECEIVED         -1
#define ZM_ERROR_PACKAGEOUTOFBOUND    -2
#define ZM_ERROR_PACKAGESIZEMISMATCH  -3
#define ZM_RETURN_OK                  0

#define ZM_BUFFER_SIZE    128

class ZMCamera {
  byte _id;
  byte camBuffer[ZM_BUFFER_SIZE];
  uint16_t bufferLen;
  boolean _debug;
  Stream * camStream;
  uint32_t _snapshotsize;
  uint16_t _snapshotpackagecount;
  uint16_t _snapshotpackagesize;
  byte * _snapshotpackatedataptr;
  uint16_t _snapshotpackatedatalen;
  uint16_t _baudrate;
  int _error;

  public:
    // CONSTRUCTION AND CONFIG
    ZMCamera(Stream * str);          // constructor of the class, uses Stream to communicate with the Camera (Serial1 / SoftwareSerial)
    void setDebug(boolean debug);    // set the lib on verbose mode (will output many lines on Serial)
    boolean setID(byte id);          // sets the ID of the camera to be queried

    // FUNCTIONS
    uint16_t helloCam();                                             // says hello and send me your version string
    boolean changeBaud(byte baud);                                   // changes the baud of the comm port with the CAM (bauds have to be begin() again after that)
    boolean changeID(byte new_id);                                   // change the ID of the camera
    boolean changeCompressionRatio(byte ratio);                      // change the compression ratio (20 to 250, default 150 or 160)
    boolean captureImage(byte resolution, uint16_t package_size);    // asks the camera to capture a snaptop
    uint16_t getPackage(uint16_t id);                                // ask to receive a package from the camera, and receive it (returns the size of the data acquired)

    // CAPTURE FUNCTIONS
    uint32_t getSnapshotSize();                           // gets the size of the snapshot
    uint16_t getPackageCount();                           // gets the number of packages needed to acquire the snapshot
    uint16_t processAllPackagesToFile(Stream * output);   // processes the reception of all packages and streams the data to output (typically, a file)
    byte * getPackagePTR();                               // gets a pointer to the package data
    uint16_t getPackageLength();                          // gets the length of the package data

    // RAW COMMUNICATION FUNCTIONS
    void sendMessage(byte * msg, int len);                          // sends a message to the camera (no reading included)
    uint16_t receiveData(int expSize, unsigned long int timeout);   // reads a given amount of data (and no more), waiting limited by timeout
    uint16_t receiveData(unsigned long int timeout);                // reads some data (length a priori unknown), waiting limited by timeout
    boolean bufferFitAnswer(byte * answer, int len);                // checks if the buffer is equal to the given message (for testing ACK messages)
    void bufferReset();                                             // resets the buffer

    // ERROR HANDLING
    int getErrorCode();    // returns the current error code

    // UTILITIES
    void displayBuffer();  // displays the buffer on Serial
};

#endif /* _ZMCAMERALIB_H_ */
