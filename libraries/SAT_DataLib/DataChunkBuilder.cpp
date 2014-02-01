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
#include "datalib_defs.h"
#include "datalib_syntax.h"
#include "DataChunkBuilder.h"


DataChunkBuilder::DataChunkBuilder(byte * buffer) {
      _packet = buffer;
      _length = 0;
      _datatypes = 0;
      _header = (struct packet_chunk_header *)_packet;
};

DataChunkBuilder::DataChunkBuilder(uint16_t datatypes, byte * buffer) {
      _packet = buffer;
      _length = computeChunkSize(datatypes);
      _datatypes = datatypes;
      _header = (struct packet_chunk_header *)_packet;
};
    
void DataChunkBuilder::init(uint16_t datatypes) {
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

