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
#include "DataChunkDecoder.h"
#include "datalib_syntax.h"
#include "SAT_DataLib.h"
#include <SD.h>

const char * datatype_names[] = {
  "MS",
  "SAT_LUM1",
  "SAT_LUM2",
  "SAT_MAG",
  "SAT_TMP1",
  "SAT_TMP2",
  "SAT_TMP3",
  "SAT_TMP4",
  "SAT_INFRATHERM",
  "SAT_ACCEL",
  "SAT_GYRO",
  "SAT_GEIGER1",
  "SAT_GEIGER2",
  "CRC16"
};



int DataChunkDecoder::getChunkLength(byte * buffer) {
      struct packet_chunk_header * t_ptr = (struct packet_chunk_header *)buffer;
      return(computeChunkSize(t_ptr->datatypes));
};

boolean DataChunkDecoder::parseChunk(byte * buffer) {
        struct packet_chunk_header * t_ptr = (struct packet_chunk_header *)buffer;
        
        int current_index = PACKET_SIZE_CHUNKHEADER;
        
        for (int i=0; i<DATATYPE_KNOWNCOUNT; i++) {
          if (t_ptr->datatypes & alldatatypes[i]) {
            onDatatype(alldatatypes[i], buffer + current_index);
            current_index += alldatatypes_lenghts[i];
          }
        }
};

boolean DataChunkDecoder::parseFile(File &dataFile, byte * buffer, int bufferlen) {
  // if we're there, it means the file is open
  while (dataFile.available() > PACKET_SIZE_CHUNKHEADER) {
    buffer[0] = dataFile.read();
    if (buffer[0] == PACKET_HEADER_CHUNK) {
      buffer[1] = dataFile.read();
      buffer[2] = dataFile.read();
      
      int expectedLen = getChunkLength(buffer);
      
      for (int i=PACKET_SIZE_CHUNKHEADER; i<expectedLen; i++)
        if (dataFile.available())
          buffer[i] = dataFile.read();
        else {
          Serial.println("!!! NOT ENOUGH DATA");
          return(false);
        }
        
      parseChunk(buffer);
    } else {
      Serial.println("!!! HEADER MISMATCH");
      continue;
    }
  }
  dataFile.close();
  return(true);
};


void DataChunkDecoder::onDatatype(uint16_t type, byte * ptr) {
  switch(type) {
    case DATATYPE_MS: {
      onMS(*((unsigned long int *)ptr));
      break;
    }
    default:
      onUnknown(type, ptr);
      break;
  };
};

