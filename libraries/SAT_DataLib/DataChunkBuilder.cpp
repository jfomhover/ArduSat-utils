/*
    Author :       Jean-Francois Omhover (@jfomhover)
    Last Changed : Jan. 12, 2013
    Description :  library for handling, formatting and decoding data from the ArduSat sensors.
                   it has been thought as a utility for sending packets through the SAT_AppStorage library

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
  uint16_t tsl_one_values[2];  // DATATYPE_SAT_LUM1
  uint16_t tsl_two_values[2];  // DATATYPE_SAT_LUM2
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

