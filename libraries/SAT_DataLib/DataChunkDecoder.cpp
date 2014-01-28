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
#include "DataChunkDecoder.h"
#include "datalib_syntax.h"
#include "SAT_DataLib.h"


const char * datatype_names[] = {
  "MS",
  "SAT_LUM1-IR;SAT_LUM1-FULL",
  "SAT_LUM2-IR;SAT_LUM2-FULL",
  "SAT_MAG-X;SAT_MAG-Y;SAT_MAG-Z",
  "SAT_TMP1",
  "SAT_TMP2",
  "SAT_TMP3",
  "SAT_TMP4",
  "SAT_INFRATHERM",
  "SAT_ACCEL-X;SAT_ACCEL-Y;SAT_ACCEL-Z",
  "SAT_GYRO-X;SAT_GYRO-Y;SAT_GYRO-Z",
  "SAT_GEIGER1",
  "SAT_GEIGER2",
  "USERDEFINED1",
  "USERDEFINED2",
  "CRC16"
};

DataChunkDecoder::DataChunkDecoder() {
	this->init();
};

void DataChunkDecoder::init() {
  header = NULL;
  currentDatatypes = 0;
  _separation = PACKET_DECODER_SEPARATIONDEFAULT;
};

void DataChunkDecoder::setSeparation(char c) {
	_separation = c;
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
			// replace ';' by user-set separation char
			//			Serial.print(datatype_names[i]);
			int j = 0;
			const char * t_str = datatype_names[i];
			while(t_str[j] > 0) {
				if (t_str[j] == PACKET_DECODER_SEPARATIONDEFAULT)
					Serial.print(_separation);
				else
					Serial.print(t_str[j]);
				j++;
			}
			Serial.print(_separation);
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
    case DATATYPE_USERDEFINED1: {
      onUserDefined(ptr);
      break;
    }
    case DATATYPE_USERDEFINED2: {
      onUserDefined(ptr);
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

/* The serie of the output easy */
void DataChunkDecoder::onMS(unsigned long int ms) { Serial.print(ms); Serial.print(_separation); };
void DataChunkDecoder::onLUM1(uint16_t lum, uint16_t ir) { Serial.print(lum); Serial.print(_separation); Serial.print(ir); Serial.print(_separation); };
void DataChunkDecoder::onLUM2(uint16_t lum, uint16_t ir) { Serial.print(lum); Serial.print(_separation); Serial.print(ir); Serial.print(_separation); };
void DataChunkDecoder::onMAG(int16_t x, int16_t y, int16_t z) { Serial.print(x); Serial.print(_separation); Serial.print(y); Serial.print(_separation); Serial.print(z); Serial.print(_separation); };
void DataChunkDecoder::onTMP1(int16_t tmp) { Serial.print(tmp); Serial.print(_separation); };
void DataChunkDecoder::onTMP2(int16_t tmp) { Serial.print(tmp); Serial.print(_separation); };
void DataChunkDecoder::onTMP3(int16_t tmp) { Serial.print(tmp); Serial.print(_separation); };
void DataChunkDecoder::onTMP4(int16_t tmp) { Serial.print(tmp); Serial.print(_separation); };
void DataChunkDecoder::onINFRATHERM(int16_t infrat) { Serial.print(infrat); Serial.print(_separation); };
void DataChunkDecoder::onACCEL(int16_t x, int16_t y, int16_t z) { Serial.print(x); Serial.print(_separation); Serial.print(y); Serial.print(_separation); Serial.print(z); Serial.print(_separation); };
void DataChunkDecoder::onGYRO(int16_t x, int16_t y, int16_t z) { Serial.print(x); Serial.print(_separation); Serial.print(y); Serial.print(_separation); Serial.print(z); Serial.print(_separation); };
void DataChunkDecoder::onUnknown(uint16_t type, byte * ptr) { Serial.print("unknown"); Serial.print(_separation); };

/* dealing with userdefined block depending on specified type */
void DataChunkDecoder::onUserDefined(byte userblock[5]) {
	// TODO : switch different types
	switch(userblock[0]) {
	case DATATYPE_USERDEFINED_STR:
    	for(int i=1; i<5; i++) {
    		char c = (char)userblock[i];
   			Serial.print(c);
    	}
    	break;
	case DATATYPE_USERDEFINED_LONGINT:
		Serial.print(*(long int *)(userblock+1));
		break;
	case DATATYPE_USERDEFINED_INTEGER:
		Serial.print(*(int *)(userblock+1));
		break;
	case DATATYPE_USERDEFINED_FLOAT:
		Serial.print(*(float *)(userblock+1));
		break;
	case DATATYPE_USERDEFINED_HEX4:
	default:
    	for(int i=1; i<5; i++) {
    		byte b = userblock[i];
    		if (b < 0x10)
    			Serial.print('0');
    		Serial.print(b,HEX);
    	}
	}
	Serial.print(_separation);
}

