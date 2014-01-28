#ifndef _DATALIB_USERDEFINED_H_
#define _DATALIB_USERDEFINED_H_

struct packet_chunk_userdefined {
  byte type;
  byte value[4];
};

#define DATATYPE_USERDEFINED_HEX4		0
#define DATATYPE_USERDEFINED_HEX3		1
#define DATATYPE_USERDEFINED_HEX2		2
#define DATATYPE_USERDEFINED_HEX1		3
#define DATATYPE_USERDEFINED_INT4		4
#define DATATYPE_USERDEFINED_LONGINT	4
#define DATATYPE_USERDEFINED_INT3		5
#define DATATYPE_USERDEFINED_INT2		6
#define DATATYPE_USERDEFINED_INTEGER	6
#define DATATYPE_USERDEFINED_INT1		7
#define DATATYPE_USERDEFINED_UINT4		8
#define DATATYPE_USERDEFINED_UINT3		9
#define DATATYPE_USERDEFINED_UINT2		10
#define DATATYPE_USERDEFINED_UINTEGER	10
#define DATATYPE_USERDEFINED_UINT1		11
#define DATATYPE_USERDEFINED_BYTE		11
#define DATATYPE_USERDEFINED_FLOAT		12
#define DATATYPE_USERDEFINED_STR		13

byte * chunkUserdefinedSetFloat(byte array[5], float f);
byte * chunkUserdefinedSetInt16(byte array[5], int16_t i);
byte * chunkUserdefinedSetUInt16(byte array[5], uint16_t i);
byte * chunkUserdefinedSetByte(byte array[5], byte b);

#endif
