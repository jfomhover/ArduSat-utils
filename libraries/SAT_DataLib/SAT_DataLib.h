/*
    File :         SAT_Datalib.h
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

#ifndef _SAT_DATALIB_H_
#define _SAT_DATALIB_H_

#define PACKET_HEADER_CHUNK  '#'
#define PACKET_HEADER_SERIE  '!'

#define PACKET_SIZE_CHUNKHEADER     3
#define PACKET_SIZE_CHUNKBODYMAX    37
#define PACKET_SIZE_HEADER          3
#define PACKET_SIZE_MAX             40

#define PACKET_UNIT_TYPE_UNDEF  0x00
#define PACKET_UNIT_TYPE_BYTE  0x01
#define PACKET_UNIT_TYPE_SHORTINT  0x02
#define PACKET_UNIT_TYPE_INT  0x03
#define PACKET_UNIT_TYPE_UINT  0x04
#define PACKET_UNIT_TYPE_LONG  0x05
#define PACKET_UNIT_TYPE_ULONG  0x06
#define PACKET_UNIT_TYPE_FLOAT  0x07

#define PACKET_SERIE_INDEX_BYTE  0x10
#define PACKET_SERIE_INDEX_UINT  0x20
#define PACKET_SERIE_INDEX_ULONG 0x30
#define PACKET_SERIE_INDEX_FLOAT 0x40

#include <Arduino.h>

#include "datalib_syntax.h"
#include "DataChunkBuilder.h"
#include "DataChunkDecoder.h"

#endif /* _SAT_DATALIB_H_ */
