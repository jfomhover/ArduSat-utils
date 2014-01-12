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

#ifndef _DATACHUNKBUILDER_H_
#define _DATACHUNKBUILDER_H_

#include <Arduino.h>
#include <util/crc16.h>
#include "datalib_syntax.h"

/* NOTE : for memory only
struct packet_chunk_header {
  char header;          // don't touch that, or adapt it, whatever ^^
  uint16_t datatypes;
};
*/

class DataChunkBuilder {
  private :
    byte * _packet;                        // buffer to the packet structure
    uint16_t _datatypes;                   // code of the available data types
    int _length;                           // length of the current packet
    struct packet_chunk_header * _header;  // struct of the packet header (at the beginning of the buffer)
    
  public :
    DataChunkBuilder(uint16_t datatypes, byte * buffer);   // construct the class and aligns the buffer with the given datatypes
    void reinit(uint16_t datatypes);                       // reconstructs the class and realigns the internal buffer using the new given datatypes
    void open();                                           // initializes the packet
    void close();                                          // closes the packet (CRC16 if needed)
    void set(uint16_t type, byte * value);                 // sets the datatype (index "type") with the value at pointer "value"
    byte * getPtr(uint16_t type);                          // returns the pointer to the datatype
    int getLength();                                       // returns the current total length of the packet
};

#endif /* _DATACHUNKBUILDER_H_ */

