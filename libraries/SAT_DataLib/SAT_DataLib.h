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

#ifndef _SAT_DATALIB_H_
#define _SAT_DATALIB_H_

#define PACKET_HEADER_CHUNK  '#'
#define PACKET_HEADER_SERIE  '!'
#define PACKET_HEADER_USERPACKET	'U'

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
#include "DataDecoder.h"

#endif /* _SAT_DATALIB_H_ */
