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

Description :  example use of SAT_AppStorage emulator (SD version)
sends a "chunk" via the DataChunkBuilder class

********************************************************************
*/

#include <SAT_DataLib.h>

byte buffer[PACKET_SIZE_MAX];    // allocate some space for the builder to work

// use the syntax beloc to declare the datatypes you'll need
DataChunkBuilder builder(  DATATYPE_MS            // datatype for millis
                           | DATATYPE_SAT_TMP1    // datatype for temp sensor 1
                           | DATATYPE_SAT_TMP2,   // datatype for temp sensor 2
                           buffer );

/* NOTE :
  the chunk will look like (hex) : 23 3100 MMMMMMMM XXXX YYYY
  23 : constant header for a chunk
  3100 : DATATYPE_MS=0x0001 | DATATYPE_SAT_TMP1=0x0x0010 | DATATYPE_SAT_TMP2=0x0020 => 0x0031, stored in little endian = 31 00
  MMMMMMMM : millis, stored as an unsigned long int (4 bytes)
  XXXX : temp 1 stored as an unsigned short int (raw value)
  XXXX : temp 2 stored as an unsigned short int (raw value)
*/

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
}


void loop() {
  // put your main code here, to run repeatedly: 

  builder.open();    // opens the builder for a new build
  
  int temp1 = 25;        // somewhere here, the values are pulled from sensors
  int temp2 = 320;
  unsigned long int t_ms = millis();
  
  builder.set(DATATYPE_MS, (byte*)&t_ms);  // the values are sent to the builder
  builder.set(DATATYPE_SAT_TMP1, (byte*)&temp1);  // the values are sent to the builder
  builder.set(DATATYPE_SAT_TMP2, (byte*)&temp2);  // the values are sent to the builder
  
  builder.close();  // the chunk is closed (final steps, and CRC (if needed))
  // NOTE : the packet is now ready to be sent to SAT_AppStorage
  // SAT_AppStorage function send(buffer, 0, builder.getLength());

  // the procedure below is just for testing
  Serial.print("ms : ");              // display temp1
  dumphex((byte*)&t_ms, sizeof(t_ms));
  Serial.print("temp1 : ");              // display temp1
  dumphex((byte*)&temp1, sizeof(temp1));
  Serial.print("temp2 : ");              // display temp1
  dumphex((byte*)&temp2, sizeof(temp2));
  
  Serial.print("packet[len=");           // display the packet
  Serial.print(builder.getLength());
  Serial.print("] : ");
  dumphex(buffer, builder.getLength());
 
  delay(2000);
}

