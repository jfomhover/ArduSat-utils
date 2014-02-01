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

#ifndef _DATACHUNKBUILDER_H_
#define _DATACHUNKBUILDER_H_

#include <Arduino.h>

class DataChunkBuilder {
  private :
    byte * _packet;                        // buffer to the packet structure
    uint16_t _datatypes;                   // code of the available data types
    int _length;                           // length of the current packet
    struct packet_chunk_header * _header;  // struct of the packet header (at the beginning of the buffer)
    
  public :
    DataChunkBuilder(byte * buffer);						// constructor with external buffer
    DataChunkBuilder(uint16_t datatypes, byte * buffer);   // construct the class and aligns the buffer with the given datatypes
    void init(uint16_t datatypes);                       // reconstructs the class and realigns the internal buffer using the new given datatypes
    void open();                                           // initializes the packet
    void close();                                          // closes the packet (CRC16 if needed)
    void set(uint16_t type, byte * value);                 // sets the datatype (index "type") with the value at pointer "value"
    byte * getPtr(uint16_t type);                          // returns the pointer to the datatype
    int getLength();                                       // returns the current total length of the packet
};

#endif /* _DATACHUNKBUILDER_H_ */

