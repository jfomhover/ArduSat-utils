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
#define PACKET_HEADER_SERIE  		'!'
#define PACKET_HEADER_USERPACKET	'U'
#define PACKET_HEADER_LOGPACKET  	'L'


// ********************
// *** PACKET SIZES ***
// ********************

#define PACKET_SIZE_CHUNKHEADER		3
#define PACKET_SIZE_CHUNKSIZEMAX	55
#define PACKET_SIZE_USERHEADER		2
#define PACKET_SIZE_LOGHEADER		2
#define PACKET_SIZE_SERIEHEADER		5
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

// SERIE PACKET
struct packet_serie_header {
  char header;  // '!'
  uint8_t keyType:4;	// unit type of the key
  uint8_t keyDim:4;		// dimensionality of the key
  uint8_t valType:4;	// unit type of the value
  uint8_t valDim:4;		// dimensionality of the value
  uint16_t length;	// length of the serie array
};

// **************************
// *** USER DEFINED TYPES ***
// **************************

// schema for these constants is 			0000XXLL XX for type, LL for len-1
#define DATATYPE_UNITTYPE_HEX	0x00
#define DATATYPE_UNIT_HEX8		0x00	//	00000000
#define DATATYPE_UNIT_HEX16		0x01	//	00000001
#define DATATYPE_UNIT_HEX24		0x02	//	00000010
#define DATATYPE_UNIT_HEX32		0x03	//	00000011

#define DATATYPE_UNITTYPE_INT	0x04
#define DATATYPE_UNIT_INT8		0x04	//	00000100
#define DATATYPE_UNIT_INT16		0x05	//	00000101
#define DATATYPE_UNIT_INT24		0x06	//	00000110
#define DATATYPE_UNIT_INT32		0x07	//	00000111

#define DATATYPE_UNITTYPE_UINT	0x08
#define DATATYPE_UNIT_UINT8		0x08	//	00001000
#define DATATYPE_UNIT_UINT16		0x09	//	00001001
#define DATATYPE_UNIT_UINT24		0x0A	//	00001010
#define DATATYPE_UNIT_UINT32		0x0B	//	00001011

#define DATATYPE_UNIT_STR		0x0D	//	00001101 EXCEPTION TO THE SCHEMA
#define DATATYPE_UNIT_FLOAT		0x0F	//  00001111

// utility constants
#define DATATYPE_UNIT_LONGINT	0x07	// (INT4)
#define DATATYPE_UNIT_SHORTINT	0x05	// (INT2)
#define DATATYPE_UNIT_INTEGER	0x05	// (SHORTINT) (INT2)
#define DATATYPE_UNIT_ULONGINT	0x0B	// (UINT4)
#define DATATYPE_UNIT_USHORTINT	0x09	// (UINT2)
#define DATATYPE_UNIT_BYTE		0x00	// (HEX1)


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
