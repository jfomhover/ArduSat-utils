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

#ifndef _DATADECODER_H_
#define _DATADECODER_H_

#include <Arduino.h>
#include "datalib_datatypes.h"
#include "datalib_userdefined.h"
#define PACKET_SIZE_CHUNKHEADER  3
#define PACKET_DECODER_SEPARATIONDEFAULT	';'

class DataDecoder {
  struct packet_chunk_header * header;
  uint16_t currentDatatypes;
  char _separation;
  public:
    DataDecoder();
    void init();
    void setSeparation(char c);
    int getChunkLength(byte * buffer);
    boolean parseChunk(byte * buffer);
    boolean parseUserPacket(byte * buffer);
    boolean parseFile(Stream &dataFile, byte * buffer, int bufferlen);
    virtual void onDataChange(uint16_t newtypes);
    virtual void onDatatype(uint16_t type, byte * ptr);  // what we should do when a datatype is detected    
    virtual void onMS(unsigned long int ms);
    virtual void onLUM1(uint16_t lum, uint16_t ir);
    virtual void onLUM2(uint16_t lum, uint16_t ir);
    virtual void onMAG(int16_t x, int16_t y, int16_t z);
    virtual void onTMP1(int16_t tmp);
    virtual void onTMP2(int16_t tmp);
    virtual void onTMP3(int16_t tmp);
    virtual void onTMP4(int16_t tmp);
    virtual void onINFRATHERM(int16_t infrat);
    virtual void onACCEL(int16_t x, int16_t y, int16_t z);
    virtual void onGYRO(int16_t x, int16_t y, int16_t z);
    virtual int onUserDefined(byte userblock[]);
    virtual void onUnknown(uint16_t type, byte * ptr);
};

#endif /* _DATADECODER_H_ */
