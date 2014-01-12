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

#ifndef _DATALIB_SYNTAX_H_
#define _DATALIB_SYNTAX_H_

#include <Arduino.h>
#include "datalib_datatypes.h"

struct packet_chunk_header {
  char header;          // don't touch that, or adapt it, whatever ^^
  uint16_t datatypes;
};

uint16_t getDatatype(int i);
int computeChunkSize(uint16_t types);
int getDatatypeIndex(uint16_t type, uint16_t packettypes);
int getDatatypeSize(uint16_t type);

extern uint16_t alldatatypes[DATATYPE_KNOWNCOUNT];
extern const int alldatatypes_lenghts[DATATYPE_KNOWNCOUNT];

#endif /* _DATALIB_SYNTAX_H_ */
