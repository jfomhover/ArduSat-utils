#include <Arduino.h>
#include "datalib_userdefined.h"

byte * chunkUserdefinedSetFloat(byte array[5], float f) {
	struct packet_chunk_userdefined * chunk = (struct packet_chunk_userdefined *)array;
	chunk->type = DATATYPE_USERDEFINED_FLOAT;
	memcpy(chunk->value, &f, 4);
};

byte * chunkUserdefinedSetInt16(byte array[5], int16_t i) {
	struct packet_chunk_userdefined * chunk = (struct packet_chunk_userdefined *)array;
	chunk->type = DATATYPE_USERDEFINED_INTEGER;
	memcpy(chunk->value, &i, 2);
};

byte * chunkUserdefinedSetUInt16(byte array[5], uint16_t i) {
	struct packet_chunk_userdefined * chunk = (struct packet_chunk_userdefined *)array;
	chunk->type = DATATYPE_USERDEFINED_UINTEGER;
	memcpy(chunk->value, &i, 2);
};

byte * chunkUserdefinedSetByte(byte array[5], byte b) {
	struct packet_chunk_userdefined * chunk = (struct packet_chunk_userdefined *)array;
	chunk->type = DATATYPE_USERDEFINED_BYTE;
	memcpy(chunk->value, &b, 1);
};
