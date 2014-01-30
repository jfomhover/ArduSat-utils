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
#include <Wire.h>
#include <EEPROM.h>
#include <OnboardCommLayer.h>
#include <SAT_Mag.h>
#include <SAT_Accel.h>
#include <SAT_Gyro.h>
#include <SAT_DataLib.h>

// *************************
// *** EXPERIMENT CONFIG ***
// *************************

#define DEBUG_MODE              // prints the values via Serial
//#define ARDUSAT_PERIOD 5552   // approx from supposed TLEs
#define PULL_DELAY 100          // data is pooled every PULL_DELAY milliseconds
#define PULL_SAMPLESIZE  64     // data is compiled (average+variance) every PULL_SAMPLESIZE iterations
                                // a compiled data packer takes 64 bytes of data
                                // that should bring 160 packets of compiled sensor values (in the 10ko limit)
                                // a delay of 543 ms in pull delay then should cover approx 1 rotation around earth (543*64*160)

// NOTE : configuration of the debug mode for writing on SD card
#define CS_PIN      4    // 4 for Arduino Ethernet
#define SD_FILENAME  "magvar.bin"


// **********************
// *** STORAGE HEADER ***
// **********************

#ifndef DEBUG_MODE
#include <SAT_AppStorage.h>
#else
#include <SD.h>
#include <SAT_AppStorageEMUSD.h>  // experiment data channel (on SD, see library at https://github.com/jfomhover/ArduSat-utils)
#endif

  
// *********************
// *** SDK INSTANCES ***
// *********************

#ifndef DEBUG_MODE
SAT_AppStorage store;       // experiment data channel (via comm)
#else
SAT_AppStorageEMUSD store;    // experiment data channel (via SD, see library at https://github.com/jfomhover/ArduSat-utils )
#endif

SAT_Mag mag;
//SAT_Accel accel;
//SAT_Gyro gyro;


// ***********************
// *** DATA STRUCTURES ***
// ***********************

// structured packet of data, easy to decode after reception (see https://github.com/jfomhover/ArduSat-utils for syntax)
struct _dataList {
  char header;
  uint8_t len;
  
  uint8_t formatMS;
      uint32_t ms;
      
  uint8_t format0;
      int16_t mag_x;
  uint8_t format1;
      int16_t mag_x_min;
  uint8_t format2;
      int16_t mag_x_max;
  uint8_t format3;
      float mag_x_avg;
  uint8_t format4;
      float mag_x_var;
      
  uint8_t format5;
      int16_t mag_y;
  uint8_t format6;
      int16_t mag_y_min;
  uint8_t format7;
      int16_t mag_y_max;
  uint8_t format8;
      float mag_y_avg;
  uint8_t format9;
      float mag_y_var;
      
  uint8_t format10;
      int16_t mag_z;
  uint8_t format11;
      int16_t mag_z_min;
  uint8_t format12;
      int16_t mag_z_max;
  uint8_t format13;
      float mag_z_avg;
  uint8_t format14;
      float mag_z_var;
} data;

#define PACKET_SIZE sizeof(struct _dataList)


// erasing all values in the data chunk
void resetDataPacket() {
  byte * t_ptr = (byte*)&data;
  for (int i=0; i<PACKET_SIZE; i++)
    t_ptr[i] = 0x00;
}


// ************************
// *** SENSOR FUNCTIONS ***
// ************************

// SETUP OF THE SENSORS NEEDED (constructors mainly)
void setupSensors() {
  mag.configMag();          // turn the MAG3110 on

//  accel.powerOn();
//  accel.setRangeSetting(2);
//  accel.setFullResBit(true);

//  gyro.reset();
//  gyro.init(I2C_ADD_GYR);
}

int count = 0;
int16_t mag_x_array[PULL_SAMPLESIZE];
int16_t mag_y_array[PULL_SAMPLESIZE];
int16_t mag_z_array[PULL_SAMPLESIZE];

void resetValues() {
  count = 0;
  for(int i=0; i<PULL_SAMPLESIZE; i++) {
    mag_x_array[i] = 0;
    mag_y_array[i] = 0;
    mag_z_array[i] = 0;
  }
}

// pulling values from the sensors and filling the data structure
void pullValues() {
//  Serial.println("pullValues()");
  mag_x_array[count] = mag.readx();
  mag_y_array[count] = mag.ready();
  mag_z_array[count] = mag.readz();
  count++;
}

void buildDataPacket() {
  data.header = 'U'; //PACKET_HEADER_USERPACKET;
  data.len = PACKET_SIZE;

  data.formatMS = DATATYPE_USERDEFINED_UINT4;
  data.ms=millis();
  
  // MAG_X
  data.format0 = DATATYPE_USERDEFINED_INT2;
  data.mag_x = mag_x_array[count-1];

  data.format1 = DATATYPE_USERDEFINED_INT2;
  data.mag_x_min = mag_x_array[0];
  for (int i=1; i<count; i++)
    if (mag_x_array[i] < data.mag_x_min)
      data.mag_x_min = mag_x_array[i];

  data.format2 = DATATYPE_USERDEFINED_INT2;
  data.mag_x_max = mag_x_array[0];
  for (int i=1; i<count; i++)
    if (mag_x_array[i] > data.mag_x_max)
      data.mag_x_max = mag_x_array[i];

  data.format3 = DATATYPE_USERDEFINED_FLOAT;
  float avg = 0;
  for (int i=0; i<count; i++)
    avg += mag_x_array[i];
  avg = avg / count;
  data.mag_x_avg = avg;
  
  data.format4 = DATATYPE_USERDEFINED_FLOAT;
  float var = 0;
  for (int i=0; i<count; i++)
    var += (mag_x_array[i]-avg)*(mag_x_array[i]-avg);
  var = var / count;
  data.mag_x_var = var;


  // MAG_Y
  data.format5 = DATATYPE_USERDEFINED_INT2;
  data.mag_y = mag_y_array[count-1];

  data.format6 = DATATYPE_USERDEFINED_INT2;
  data.mag_y_min = mag_y_array[0];
  for (int i=1; i<count; i++)
    if (mag_y_array[i] < data.mag_y_min)
      data.mag_y_min = mag_y_array[i];

  data.format7 = DATATYPE_USERDEFINED_INT2;
  data.mag_y_max = mag_y_array[0];
  for (int i=1; i<count; i++)
    if (mag_y_array[i] > data.mag_y_max)
      data.mag_y_max = mag_y_array[i];

  data.format8 = DATATYPE_USERDEFINED_FLOAT;
  avg = 0;
  for (int i=0; i<count; i++)
    avg += mag_y_array[i];
  avg = avg / count;
  data.mag_y_avg = avg;
  
  data.format9 = DATATYPE_USERDEFINED_FLOAT;
  var = 0;
  for (int i=0; i<count; i++)
    var += (mag_y_array[i]-avg)*(mag_y_array[i]-avg);
  var = var / count;
  data.mag_y_var = var;


  // MAG_Z
  data.format10 = DATATYPE_USERDEFINED_INT2;
  data.mag_z = mag_z_array[count-1];

  data.format11 = DATATYPE_USERDEFINED_INT2;
  data.mag_z_min = mag_z_array[0];
  for (int i=1; i<count; i++)
    if (mag_z_array[i] < data.mag_z_min)
      data.mag_z_min = mag_z_array[i];

  data.format12 = DATATYPE_USERDEFINED_INT2;
  data.mag_z_max = mag_z_array[0];
  for (int i=1; i<count; i++)
    if (mag_z_array[i] > data.mag_z_max)
      data.mag_z_max = mag_z_array[i];

  data.format13 = DATATYPE_USERDEFINED_FLOAT;
  
  avg = 0;
  for (int i=0; i<count; i++)
    avg += mag_z_array[i];
  avg = avg / count;
  data.mag_z_avg = avg;
  
  data.format14 = DATATYPE_USERDEFINED_FLOAT;
  var = 0;
  for (int i=0; i<count; i++)
    var += (mag_z_array[i]-avg)*(mag_z_array[i]-avg);
  var = var / count;
  data.mag_z_var = var;
}


// ******************
// *** MAIN SETUP ***
// ******************

void setup() {

#ifdef DEBUG_MODE
  Serial.begin(9600);
  store.init( true, CS_PIN, false, SD_FILENAME ); // debug : outputing verbose lines on Serial + write result on SD file
#endif

  OBCL.begin();             // setups the communication via I2C (sensors + AppStorage)
  
  setupSensors();
  resetValues();
}


// *****************
// *** MAIN LOOP ***
// *****************
 
signed long int previousMs = 0;
signed long int nextMs = 0;
int dataSent = 0;

void loop() {

  nextMs = PULL_DELAY-(millis()-previousMs);  // "(not so) intelligent delay" : just the ms needed to have a regular timing, takes into account the delay of all the functions in the loop
  previousMs += PULL_DELAY;
  
  if (nextMs > 0)
    delay(nextMs); //wait for next pull
  else
    previousMs += PULL_DELAY; // we missed a beat, skip one to get back on regular track

  pullValues();   // pull the values needed

  if (count < PULL_SAMPLESIZE)
    return;
  
  resetDataPacket();
  buildDataPacket();
  
  byte * t_ptr = (byte *)&data;
  store.send(t_ptr, 0, PACKET_SIZE);   // sends data into the communication file and queue for transfer
                                       // WARNING : introduces a 100ms delay

  dataSent += PACKET_SIZE;
  
  resetValues();
/*
  if ((dataSent + PACKET_SIZE) > 10240) {
    // TODO : close experiments, clean mode ?
    // my understanding is : that will be done automatically when we reach the 10kb limit
  }
*/
}

