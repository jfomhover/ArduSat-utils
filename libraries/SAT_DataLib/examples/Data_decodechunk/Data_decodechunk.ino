/*
    Author :       Jean-Francois Omhover (@jfomhover)
    Last Changed : Jan. 12, 2013
    Description :  example of a "chunk" of data (timestamped set of data, raw values pulled from the ArduSat sensors)
                   here we use a prior packet to decode it using the decoding scheme

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

#include <SD.h>
#include <SAT_DataLib.h>

/* NOTE :
  the chunk will look like (hex) : 23 3100 MMMMMMMM XXXX YYYY
  23 : constant header for a chunk
  3100 : DATATYPE_MS=0x0001 | DATATYPE_SAT_TMP1=0x0x0010 | DATATYPE_SAT_TMP2=0x0020 => 0x0031, stored in little endian = 31 00
  MMMMMMMM : millis, stored as an unsigned long int (4 bytes)
  XXXX : temp 1 stored as an unsigned short int (raw value)
  XXXX : temp 2 stored as an unsigned short int (raw value)
*/
byte message1[11] = { 0x23, 0x31, 0x00, 0xAD, 0x62, 0x03, 0x00, 0x19, 0x00, 0x40, 0x01 };


// use the syntax beloc to declare the datatypes you'll need
class myDecoder : public DataChunkDecoder {
  void onDatatype(uint16_t type, byte * ptr) {
    Serial.print("datatype=");
    Serial.print(type,HEX);
  };
} decoder;


// just a function for demonstrating the functions
// this function outputs a binary message in hexadecimal on serial
void dumphex(byte * data, int len) {
  for (int i=0; i<len; i++) {
    if (data[i]<0x10) {Serial.print('0');}
      Serial.print(data[i],HEX);
      Serial.print(' ');
    }
    Serial.print('\n');
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  Serial.print("packet[len=");           // display the packet
  Serial.print(11);
  Serial.print("] : ");
  dumphex(message1, 11);

  decoder.parseChunk(message1);
}


void loop() {
  // put your main code here, to run repeatedly: 
}

