/*
    File :         SAT_AppStorageEMU.cpp
    Author(s) :    + Jean-Francois Omhover (@jfomhover)
                   + NanoSatisfi Inc.
    Last Changed : Jan. 11, 2014
    Description :  emulator class adapted from the SAT_AppStorage class made by NanoSatisfi for ArduSat
                   outputs the values to Serial instead of communicating them via I2C
                   
    Usage :        - same as SAT_AppStorage
                   - option : modify debugMode -> true to output the message to Serial

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

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <EEPROM.h>
#include "SAT_AppStorageEMU.h"
#include <avr/pgmspace.h>


// *** RAM OPTIMIZATION FOR DEBUG (too much strings in this class)
const prog_uchar string_5[] PROGMEM = "*** END OF EXPERIMENT BY REACHING 10KB";
#define PGM_STRING_ENDOFEXP		string_5
const prog_uchar string_6[] PROGMEM = "*** SAT_AppStorageEMU::copyAndSend() : ";
#define PGM_STRING_COPYANDSEND	string_6
const prog_uchar string_7[] PROGMEM = " ms=";
#define PGM_STRING_MS			string_7
const prog_uchar string_8[] PROGMEM = " node_addr=";
#define PGM_STRING_NODEADDR		string_8
const prog_uchar string_9[] PROGMEM = " prefix=";
#define PGM_STRING_PREFIX		string_9
const prog_uchar string_10[] PROGMEM = " len=";
#define PGM_STRING_LEN			string_10
const prog_uchar string_11[] PROGMEM = " type=";
#define PGM_STRING_TYPE			string_11
const prog_uchar string_12[] PROGMEM = " totalsent=";
#define PGM_STRING_TOTALLEN		string_12
const prog_uchar string_13[] PROGMEM = "Error opening ";
#define PGM_STRING_ERROROPENING	string_13
const prog_uchar string_14[] PROGMEM = "*** sending chars : ";
#define PGM_STRING_SENDINGCHARS	string_14
const prog_uchar string_15[] PROGMEM = "*** sending bytes : ";
#define PGM_STRING_SENDINGBYTES	string_15


void ASEMU_printPROGMEMString(const prog_uchar *str) {
	char c;

	while((c = pgm_read_byte(str++)))
		Serial.write(c);
}

/******************************************************************************
 * Constructors
 ******************************************************************************/
SAT_AppStorageEMU::SAT_AppStorageEMU()
{
	nodeAddress_  = EEPROM.read(0x00);
	debugMode_ = false;
	dataCount_ = 0;
}

void SAT_AppStorageEMU::init(boolean debug) {
	debugMode_ = debug;
}

/******************************************************************************
 * User API
 ******************************************************************************/
/*
   Passes the experiment data to the comm layer; the comm layer enqueues if for
   sending upon request from master;

   @note There is a 100ms delay to allow time for FS work.
*/
void SAT_AppStorageEMU::send(char data[])
{
	unsigned int dataLen  = (unsigned)strlen(data);
	unsigned int messages = dataLen / NODE_COMM_MAX_BUFFER_SIZE;

	if(debugMode_) {
		ASEMU_printPROGMEMString(PGM_STRING_SENDINGCHARS);
	}
	Serial.println(data);

	for(unsigned int i = 0; i < messages; i++)
	{
		unsigned int start_offset   = i * NODE_COMM_MAX_BUFFER_SIZE;
		copyAndSend((byte*) data, start_offset, NODE_COMM_MAX_BUFFER_SIZE);
	}

	// process remainder or if data was less then NODE_COMM_MAX_BUFFER_SIZE;
	uint8_t remainderLen = dataLen % NODE_COMM_MAX_BUFFER_SIZE;
	uint8_t finalOffset  = (dataLen > NODE_COMM_MAX_BUFFER_SIZE) ?
		(messages * NODE_COMM_MAX_BUFFER_SIZE) : 0;

	copyAndSend((byte*) data, finalOffset, remainderLen);
}

void SAT_AppStorageEMU::send(byte *data ,unsigned int offset, unsigned int length)
{
	unsigned int dataLen  = (unsigned)(length-offset);
	unsigned int messages = dataLen / NODE_COMM_MAX_BUFFER_SIZE;

	if(debugMode_) {
		ASEMU_printPROGMEMString(PGM_STRING_SENDINGBYTES);
		for (int i=offset; i<length; i++) {
			if (data[i]<0x10) {Serial.print('0');}
			Serial.print(data[i],HEX);
			Serial.print(' ');
		}
		Serial.print('\n');
	}


	for(unsigned int i = 0; i < messages; i++)
	{
		unsigned int start_offset   = i * NODE_COMM_MAX_BUFFER_SIZE;
		copyAndSend((byte*) data, offset+start_offset, NODE_COMM_MAX_BUFFER_SIZE);
	}
	// process remainder or if data was less then NODE_COMM_MAX_BUFFER_SIZE;

	uint8_t remainderLen = dataLen % NODE_COMM_MAX_BUFFER_SIZE;
	uint8_t finalOffset  = (dataLen > NODE_COMM_MAX_BUFFER_SIZE) ?
		(messages * NODE_COMM_MAX_BUFFER_SIZE) : 0;

	copyAndSend((byte*) data, offset+finalOffset, remainderLen);
}

void SAT_AppStorageEMU::copyAndSend(byte data[], unsigned int offset, unsigned int length)
{
	nanosat_message_t msg;
	msg.node_addr = nodeAddress_;
	msg.prefix    = NODE_COMM_MESSAGE_PREFIX;
	msg.len       = length;
	msg.type      = APPEND;
	memcpy(msg.buf, (uint8_t*)&(data[offset]), length * sizeof(char));
	// commLayer_.sendMessage(msg);
	dataCount_+=msg.len;

	if (dataCount_ > 10240) {
		if (debugMode_)
			ASEMU_printPROGMEMString(PGM_STRING_ENDOFEXP);
		while(1);
	}

	if (debugMode_) {
		ASEMU_printPROGMEMString(PGM_STRING_COPYANDSEND);

		ASEMU_printPROGMEMString(PGM_STRING_MS);
		Serial.print(millis());

		ASEMU_printPROGMEMString(PGM_STRING_NODEADDR);
		Serial.print(msg.node_addr,HEX);

		ASEMU_printPROGMEMString(PGM_STRING_PREFIX);
		Serial.print(msg.prefix,HEX);

		ASEMU_printPROGMEMString(PGM_STRING_LEN);
		Serial.print(msg.len);

		ASEMU_printPROGMEMString(PGM_STRING_TYPE);
		Serial.print(msg.type,HEX);

		ASEMU_printPROGMEMString(PGM_STRING_TOTALLEN);
		Serial.println(dataCount_);
	}

	delay(100);
}
