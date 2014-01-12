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
#include <SD.h>

#define PACKET_SIZE_CHUNKHEADER  3


class DataChunkDecoder {
  public:
    int getChunkLength(byte * buffer);
    boolean parseChunk(byte * buffer);
    boolean parseFile(File &dataFile, byte * buffer, int bufferlen);
    virtual void onDatatype(uint16_t type, byte * ptr);  // what we should do when a datatype is detected    
    virtual void onMS(unsigned long int ms) {};
    virtual void onLUM1(uint16_t lum, uint16_t ir) {};
    virtual void onLUM2(uint16_t lum, uint16_t ir) {};
    virtual void onMAG(int16_t x, int16_t y, int16_t z) {};
    virtual void onTMP1(int16_t tmp) {};
    virtual void onTMP2(int16_t tmp) {};
    virtual void onTMP3(int16_t tmp) {};
    virtual void onTMP4(int16_t tmp) {};
    virtual void onINFRATHERM(int16_t infrat) {};
    virtual void onACCEL(int16_t x, int16_t y, int16_t z) {};
    virtual void onGYRO(int16_t x, int16_t y, int16_t z) {};
    virtual void onUnknown(uint16_t type, byte * ptr) {};
};

#endif /* _DATACHUNKDECODER_H_ */
