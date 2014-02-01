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

    Description :  definitions, constants for the SAT_DataLib

********************************************************************
*/

#ifndef DATALIB_STRUCTS_H_
#define DATALIB_STRUCTS_H_

// ********************
// *** PACKET TYPES ***
// ********************

#define PACKET_HEADER_CHUNK  		'#'
//#define PACKET_HEADER_SERIE  		'!'
#define PACKET_HEADER_USERPACKET	'U'
#define PACKET_HEADER_LOGPACKET  	'S'


// ********************
// *** PACKET SIZES ***
// ********************

#define PACKET_SIZE_CHUNKHEADER		3
#define PACKET_SIZE_CHUNKSIZEMAX	55
#define PACKET_SIZE_USERHEADER		2
#define PACKET_SIZE_LOGHEADER		2
//#define PACKET_SIZE_MAX				55


// ***************
// *** HEADERS ***
// ***************

// CHUNK PACKET
struct packet_chunk_header {
  char header;	// '#'
  uint16_t datatypes;
};

// USER PACKET
struct packet_user_header {
  char header;	// 'U'
  uint8_t len;	// total length of the packet
};

// LOG PACKET
struct packet_log_header {
  char header;	// 'L'
  uint8_t len;	// total length of the packet
};


// **************************
// *** USER DEFINED TYPES ***
// **************************

#define DATATYPE_UNIT_HEX4		0x00
#define DATATYPE_UNIT_HEX3		0x01
#define DATATYPE_UNIT_HEX2		2
#define DATATYPE_UNIT_HEX1		3

#define DATATYPE_UNIT_INT4		4
#define DATATYPE_UNIT_LONGINT	4

#define DATATYPE_UNIT_INT3		5

#define DATATYPE_UNIT_INT2		6
#define DATATYPE_UNIT_SHORTINT	6
#define DATATYPE_UNIT_INTEGER	6

#define DATATYPE_UNIT_INT1		7

#define DATATYPE_UNIT_UINT4		8
#define DATATYPE_UNIT_ULONGINT	8

#define DATATYPE_UNIT_UINT3		9

#define DATATYPE_UNIT_UINT2		10
#define DATATYPE_UNIT_USHORTINT	10

#define DATATYPE_UNIT_UINT1		11
#define DATATYPE_UNIT_BYTE		11

#define DATATYPE_UNIT_FLOAT		12
#define DATATYPE_UNIT_STR		13


// ***************************
// *** CHUNK / FULL STRUCT ***
// ***************************

struct packet_chunk_full {
  char header;
  uint16_t datatypes;
  long int ms;                 // DATATYPE_MS
  uint16_t tsl_one_ir;         // DATATYPE_SAT_LUM1
  uint16_t tsl_one_full;       // DATATYPE_SAT_LUM1
  uint16_t tsl_two_ir;         // DATATYPE_SAT_LUM2
  uint16_t tsl_two_full;       // DATATYPE_SAT_LUM2
  int16_t mag[3];              // DATATYPE_SAT_MAG
  int16_t temp_value1;         // DATATYPE_SAT_TMP1
  int16_t temp_value2;         // DATATYPE_SAT_TMP2
  int16_t temp_value3;         // DATATYPE_SAT_TMP3
  int16_t temp_value4;         // DATATYPE_SAT_TMP4
  int16_t infrat_value;        // DATATYPE_SAT_INFRATHERM
  int16_t accel[3];            // DATATYPE_SAT_ACCEL
  int16_t gyro[3];             // DATATYPE_SAT_GYRO
  byte userdefinedblock[5];    // DATATYPE_USERDEFINED1
  byte userdefinedblock2[5];    // DATATYPE_USERDEFINED2
//  uint16_t crc16;              // DATATYPE_CRC16  // TODO : not available for now
};

// ****************************
// *** CHUNK / USER DEFINED ***
// ****************************

struct packet_chunk_userdefined {
  byte type;
  byte value[4];
};

#endif /* DATALIB_STRUCTS_H_ */
