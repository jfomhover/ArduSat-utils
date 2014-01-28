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

Description :  library for handling, formatting and decoding data from the ArduSat sensors.
it has been thought as a utility for sending packets through the SAT_AppStorage library

********************************************************************
*/

#include <Arduino.h>
#include <util/crc16.h>
#include "SAT_DataLib.h"
#include "datalib_syntax.h"
#include "DataChunkBuilder.h"

/* 
// NOTE : for memory only
struct packet_chunk_header {
  char header;          // don't touch that, or adapt it, whatever ^^
  uint16_t datatypes;
};

// NOTE : below, the structure model used for packets
struct _packet_chunk_struct {
  char header;          // don't touch that, or adapt it, whatever ^^
  uint16_t datatypes;

  long int ms;                 // DATATYPE_MS
  uint16_t tsl_one_ir;         // DATATYPE_SAT_LUM1
  uint16_t tsl_one_full;       // DATATYPE_SAT_LUM1
  uint16_t tsl_two_ir;         // DATATYPE_SAT_LUM2
  uint16_t tsl_two_full;       // DATATYPE_SAT_LUM2
  int16_t mag_values[3];       // DATATYPE_SAT_MAG
  int16_t temp_value1;         // DATATYPE_SAT_TMP1
  int16_t temp_value2;         // DATATYPE_SAT_TMP2
  int16_t temp_value3;         // DATATYPE_SAT_TMP3
  int16_t temp_value4;         // DATATYPE_SAT_TMP4
  int16_t infrat_value;        // DATATYPE_SAT_INFRATHERM
  int16_t accel[3];            // DATATYPE_SAT_ACCEL
  int16_t gyro[3];             // DATATYPE_SAT_GYRO

  // DATATYPE_SAT_GEIGER
// TODO : long geiger_cpm1;
// TODO : float 

  uint16_t  crc16;  // DATATYPE_CRC16
};
*/

DataChunkBuilder::DataChunkBuilder(uint16_t datatypes, byte * buffer) {
      _packet = buffer;
      _length = computeChunkSize(datatypes);
      _datatypes = datatypes;
      _header = (struct packet_chunk_header *)_packet;
};
    
void DataChunkBuilder::reinit(uint16_t datatypes) {
      _length = computeChunkSize(datatypes);
      _datatypes = datatypes;      
};
    
    // initialize the structure of the packet
void DataChunkBuilder::open() {
      _header->header = PACKET_HEADER_CHUNK;
      _header->datatypes = _datatypes;
};
    
void DataChunkBuilder::close() {
//      _header->ms = millis();
      
      if (_header->datatypes & DATATYPE_CRC16) {
        uint16_t crc16 = 0;
        uint8_t * ptr_t = (uint8_t *)&_packet;
        uint8_t size_t = _length - 2;

        for (int i=0; i<size_t; i++)
          crc16 = _crc16_update(crc16,ptr_t[i]);

        Serial.print("CRC16: ");
        Serial.println(crc16);
        set(DATATYPE_CRC16, (byte*)&crc16);
      }
};
 
void DataChunkBuilder::set(uint16_t type, byte * value) {
      if (type & _datatypes) {
        byte * t_ptr = _packet + getDatatypeIndex(type,_datatypes);
        int t_size = getDatatypeSize(type);
        if (t_size > 0)
          memcpy(t_ptr, value, t_size);
      }
};
    
    
int DataChunkBuilder::getLength() {
      return(_length);
};

