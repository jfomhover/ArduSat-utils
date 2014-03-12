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
#include "DataDecoder.h"
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

DataDecoder::DataDecoder() {
	this->init();
};

void DataDecoder::init() {
  header = NULL;
  currentDatatypes = 0;
  _separation = PACKET_DECODER_SEPARATIONDEFAULT;
};

void DataDecoder::setSeparation(char c) {
	_separation = c;
};

int DataDecoder::getChunkLength(byte * buffer) {
      struct packet_chunk_header * t_ptr = (struct packet_chunk_header *)buffer;
      return(computeChunkSize(t_ptr->datatypes));
};

boolean DataDecoder::parseChunkPacket(byte * buffer) {
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

boolean DataDecoder::parseUserPacket(byte * buffer) {
	int packetLen = buffer[1];
	int index = 2;
	while (index < packetLen) {
		index += onUserDefined(buffer+index);
	};
	Serial.print('\n');
};

boolean DataDecoder::parseLogPacket(byte * buffer) {
	Serial.print("*** ");
	onString((char*)(buffer+2), buffer[1]-2);
	Serial.print('\n');
};

int DataDecoder::computeSerieLength(byte * buffer) {
	int entryLen = computeSerieEntryLength(buffer[1])+computeSerieEntryLength(buffer[2]);

	struct packet_serie_header * t_ptr = (struct packet_serie_header*)buffer;
	return((entryLen * t_ptr->length)+sizeof(struct packet_serie_header));
}

int DataDecoder::computeSerieEntryLength(byte def) {
	int entryLen = 0;

	int keyLen = 0;
	if ((def & 0x0F) == DATATYPE_UNIT_STR)
		keyLen = 4;
	else
		keyLen = (def & 0x03) + 1; // 00000011

	int keyDim = (def & 0xF0) >> 4;

	entryLen = keyLen * keyDim;
/*
	Serial.print("entryLen=");
	Serial.println(entryLen);
*/
	return(entryLen);
}

boolean DataDecoder::parseSeriePacket(byte * buffer, int bufferlen, Stream &dataFile) {
	struct packet_serie_header * t_ptr = (struct packet_serie_header*)buffer;

/*
	Serial.print("parseSeriePacket ");
	for (int i=0; i<5; i++) {
		Serial.print(buffer[i],HEX);
		Serial.write(' ');
	}
	Serial.println(bufferlen);
*/

	int keyLen = computeSerieEntryLength(buffer[1]);
	int valLen = computeSerieEntryLength(buffer[2]);
	int entryLen = keyLen + valLen;
	byte * t_buffer = buffer + sizeof(struct packet_serie_header);
	int t_bufferlen = bufferlen - sizeof(struct packet_serie_header);

	for (int i=0; i<t_ptr->length; i++) {
		for (int j=0; j<entryLen; j++)
			t_buffer[j] = dataFile.read();
		onSerieEntry(buffer[1], t_buffer);
		onSerieEntry(buffer[2], t_buffer+keyLen);
		Serial.write('\n');
	}
}

void DataDecoder::onSerieEntry(byte def, byte * block) {
	int keyType = def & 0x0F;
	int keyDim = (def & 0xF0) >> 4;
	int keyUnitLen = (def & 0x03)+1;

	for (int i=0; i<keyDim; i++) {
		if (keyType == DATATYPE_UNIT_STR)
			onString((char*)(block+i*4), 4);
		else
			onUserDefined(keyType, block+i*keyUnitLen);
	}
}

boolean DataDecoder::parseBuffer(byte * buffer) {
	switch(buffer[0]) {
	case PACKET_HEADER_CHUNK:
		parseChunkPacket(buffer);
		break;
	case PACKET_HEADER_USERPACKET:
		parseUserPacket(buffer);
		break;
	case PACKET_HEADER_LOGPACKET:
		parseLogPacket(buffer);
		break;
/*	case PACKET_HEADER_SERIE:
		parseSeriePacket(buffer, 4096);
		break;*/
	default:
		Serial.println("!!! HEADER MISMATCH");
		return(false);
		break;
	}
	return(true);
};

boolean DataDecoder::parseFile(Stream &dataFile, byte * buffer, int bufferlen) {
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
        
      parseChunkPacket(buffer);
    } else if (buffer[0] == PACKET_HEADER_USERPACKET){
        buffer[1] = dataFile.read();
    	int expectedLen = buffer[1];
        for (int i=2; i<expectedLen; i++)
          if (dataFile.available())
            buffer[i] = dataFile.read();
          else {
            Serial.println("!!! NOT ENOUGH DATA");
            return(false);
          }
        parseUserPacket(buffer);
    } else if (buffer[0] == PACKET_HEADER_LOGPACKET) {
    	buffer[1] = dataFile.read();
    	int expectedLen = buffer[1];
        for (int i=2; i<expectedLen; i++)
          if (dataFile.available())
            buffer[i] = dataFile.read();
          else {
            Serial.println("!!! NOT ENOUGH DATA");
            return(false);
          }
        parseLogPacket(buffer);
    } else if (buffer[0] == PACKET_HEADER_SERIE) {
    	buffer[1] = dataFile.read();
    	buffer[2] = dataFile.read();
    	buffer[3] = dataFile.read();
    	buffer[4] = dataFile.read();
    	parseSeriePacket(buffer, bufferlen, dataFile);
    } else {
      Serial.println("!!! HEADER MISMATCH");
      continue;
    }
  }
  return(true);
};

void DataDecoder::onDataChange(uint16_t newtype) {
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

void DataDecoder::onDatatype(uint16_t type, byte * ptr) {
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
void DataDecoder::onMS(unsigned long int ms) { Serial.print(ms); Serial.print(_separation); };
void DataDecoder::onLUM1(uint16_t lum, uint16_t ir) { Serial.print(lum); Serial.print(_separation); Serial.print(ir); Serial.print(_separation); };
void DataDecoder::onLUM2(uint16_t lum, uint16_t ir) { Serial.print(lum); Serial.print(_separation); Serial.print(ir); Serial.print(_separation); };
void DataDecoder::onMAG(int16_t x, int16_t y, int16_t z) { Serial.print(x); Serial.print(_separation); Serial.print(y); Serial.print(_separation); Serial.print(z); Serial.print(_separation); };
void DataDecoder::onTMP1(int16_t tmp) { Serial.print(tmp); Serial.print(_separation); };
void DataDecoder::onTMP2(int16_t tmp) { Serial.print(tmp); Serial.print(_separation); };
void DataDecoder::onTMP3(int16_t tmp) { Serial.print(tmp); Serial.print(_separation); };
void DataDecoder::onTMP4(int16_t tmp) { Serial.print(tmp); Serial.print(_separation); };
void DataDecoder::onINFRATHERM(int16_t infrat) { Serial.print(infrat); Serial.print(_separation); };
void DataDecoder::onACCEL(int16_t x, int16_t y, int16_t z) { Serial.print(x); Serial.print(_separation); Serial.print(y); Serial.print(_separation); Serial.print(z); Serial.print(_separation); };
void DataDecoder::onGYRO(int16_t x, int16_t y, int16_t z) { Serial.print(x); Serial.print(_separation); Serial.print(y); Serial.print(_separation); Serial.print(z); Serial.print(_separation); };
void DataDecoder::onUnknown(uint16_t type, byte * ptr) { Serial.print("unknown"); Serial.print(_separation); };

void DataDecoder::onString(char * buffer, int len) {
	for(int i=0; i<len; i++)
		Serial.write(buffer[i]);
	Serial.print(_separation);
};

void DataDecoder::onUserDefined(byte def, byte * block) {
	int type = def & 0x0F;
	unsigned long int ui = 0;
	signed long int sli = 0;
//	float f;

	if (type == DATATYPE_UNIT_STR) {
		for (int i=0; i<4; i++)
			Serial.write((char)block[i]);
		return;
	}

	if (type == DATATYPE_UNIT_FLOAT) {
		Serial.print(*((float*)block));
	}

	int len = (def & 0x03)+1; 	// 00000011
	type = def & 0x0C;	// 00001100

	switch(type) {
	case DATATYPE_UNITTYPE_HEX:
//		memcpy((void*)&ui, (void*)block, len);
		for (int i=0; i<len; i++) {
			if (block[i] < 0x10)
				Serial.print('0');
			Serial.print(block[i],HEX);
		}
		break;
	case DATATYPE_UNITTYPE_INT:
		memcpy((void*)&sli, (void*)block, len);
		if (block[len-1] & 0x80) {
			for (int i=len; i<4; i++)
				((byte*)&sli)[i] = 0xFF;
		}
		Serial.print(sli);
		break;
	case DATATYPE_UNITTYPE_UINT:
		ui = 0;
		memcpy((void*)&ui, (void*)block, len);
		Serial.print(ui);
		break;
	default:
		Serial.print("NaN");
		break;
	}
	Serial.print(_separation);
}

/* dealing with userdefined block depending on specified type */
int DataDecoder::onUserDefined(byte userblock[]) {
	onUserDefined(userblock[0], userblock+1);

	if (userblock[0] == DATATYPE_UNIT_STR)
		return(5);
	if (userblock[0] == DATATYPE_UNIT_FLOAT)
		return(5);
	int len = (userblock[0] & 0x03)+1; 	// 00000011
	return(len);
}

