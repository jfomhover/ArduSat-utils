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

#ifndef _DATACHUNKDECODER_H_
#define _DATACHUNKDECODER_H_

#include <Arduino.h>

#define PACKET_SIZE_CHUNKHEADER  3


class DataChunkDecoder {
  struct packet_chunk_header * header;
  uint16_t currentDatatypes;
  public:
    void init();
    int getChunkLength(byte * buffer);
    boolean parseChunk(byte * buffer);
    boolean parseFile(Stream &dataFile, byte * buffer, int bufferlen);
    virtual void onDataChange(uint16_t newtypes);
    virtual void onDatatype(uint16_t type, byte * ptr);  // what we should do when a datatype is detected    
    virtual void onMS(unsigned long int ms) { Serial.print(ms); Serial.print(';'); };
    virtual void onLUM1(uint16_t lum, uint16_t ir) { Serial.print(lum); Serial.print(';'); Serial.print(ir); Serial.print(';'); };
    virtual void onLUM2(uint16_t lum, uint16_t ir) { Serial.print(lum); Serial.print(';'); Serial.print(ir); Serial.print(';'); };
    virtual void onMAG(int16_t x, int16_t y, int16_t z) { Serial.print(x); Serial.print(';'); Serial.print(y); Serial.print(';'); Serial.print(z); Serial.print(';'); };
    virtual void onTMP1(int16_t tmp) { Serial.print(tmp); Serial.print(';'); };
    virtual void onTMP2(int16_t tmp) { Serial.print(tmp); Serial.print(';'); };
    virtual void onTMP3(int16_t tmp) { Serial.print(tmp); Serial.print(';'); };
    virtual void onTMP4(int16_t tmp) { Serial.print(tmp); Serial.print(';'); };
    virtual void onINFRATHERM(int16_t infrat) { Serial.print(infrat); Serial.print(';'); };
    virtual void onACCEL(int16_t x, int16_t y, int16_t z) { Serial.print(x); Serial.print(';'); Serial.print(y); Serial.print(';'); Serial.print(z); Serial.print(';'); };
    virtual void onGYRO(int16_t x, int16_t y, int16_t z) { Serial.print(x); Serial.print(';'); Serial.print(y); Serial.print(';'); Serial.print(z); Serial.print(';'); };
    virtual void onUnknown(uint16_t type, byte * ptr) { Serial.print("unknown"); Serial.print(';'); };
};

#endif /* _DATACHUNKDECODER_H_ */
