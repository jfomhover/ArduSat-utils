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


void DataChunkDecoder::init() {
  header = NULL;
  currentDatatypes = 0;
};

int DataChunkDecoder::getChunkLength(byte * buffer) {
      struct packet_chunk_header * t_ptr = (struct packet_chunk_header *)buffer;
      return(computeChunkSize(t_ptr->datatypes));
};

boolean DataChunkDecoder::parseChunk(byte * buffer) {
        header = (struct packet_chunk_header *)buffer;
        
      if (header->datatypes != currentDatatypes) {
        onDataChange(header->datatypes);
        currentDatatypes = header->datatypes;
      }

        int current_index = PACKET_SIZE_CHUNKHEADER;
        
        for (int i=0; i<DATATYPE_KNOWNCOUNT; i++) {
          if (header->datatypes & alldatatypes[i]) {
            onDatatype(alldatatypes[i], buffer + current_index);
            current_index += alldatatypes_lenghts[i];
          }
        }
	Serial.print('\n');
};

boolean DataChunkDecoder::parseFile(Stream &dataFile, byte * buffer, int bufferlen) {
  header = (struct packet_chunk_header *)buffer;

  currentDatatypes=0;

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
  return(true);
};

void DataChunkDecoder::onDataChange(uint16_t newtype) {
	// prints out headers
	for (int i=0; i<DATATYPE_KNOWNCOUNT; i++) {
		if (newtype & alldatatypes[i]) {
			Serial.print(datatype_names[i]);
			Serial.print(';');
		}
	}
	Serial.print('\n');
};

void DataChunkDecoder::onDatatype(uint16_t type, byte * ptr) {
  switch(type) {
    case DATATYPE_MS: {
      onMS(*((unsigned long int *)ptr));
      break;
    }
    case DATATYPE_SAT_LUM1: {
      onLUM1(*((uint16_t *)ptr), *((uint16_t *)ptr+1));
      break;
    }
    case DATATYPE_SAT_LUM2: {
      onLUM2(*((uint16_t *)ptr), *((uint16_t *)ptr+1));
      break;
    }
    case DATATYPE_SAT_MAG: {
      onMAG(*((int16_t *)ptr), *((int16_t *)ptr+1), *((int16_t *)ptr+2));
      break;
    }
    case DATATYPE_SAT_TMP1: {
      onTMP1(*((int16_t *)ptr));
      break;
    }
    case DATATYPE_SAT_TMP2: {
      onTMP2(*((int16_t *)ptr));
      break;
    }
    case DATATYPE_SAT_TMP3: {
      onTMP3(*((int16_t *)ptr));
      break;
    }
    case DATATYPE_SAT_TMP4: {
      onTMP4(*((int16_t *)ptr));
      break;
    }
    case DATATYPE_SAT_INFRATHERM: {
      onINFRATHERM(*((int16_t *)ptr));
      break;
    }
    case DATATYPE_SAT_ACCEL: {
      onACCEL(*((int16_t *)ptr), *((int16_t *)ptr+1), *((int16_t *)ptr+2));
      break;
    }
    case DATATYPE_SAT_GYRO: {
      onGYRO(*((int16_t *)ptr), *((int16_t *)ptr+1), *((int16_t *)ptr+2));
      break;
    }
/*    case DATATYPE_SAT_GEIGER1: {
    }
    case DATATYPE_SAT_GEIGER2: {
    }
    case DATATYPE_CRC16: {
    }*/
    default:
      onUnknown(type, ptr);
      break;
  };
};

