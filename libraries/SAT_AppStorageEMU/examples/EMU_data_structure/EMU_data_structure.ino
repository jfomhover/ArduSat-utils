/*
    File :         EMU_data_structure.ino
    Author(s) :    + Jean-Francois Omhover (@jfomhover)
    Last Changed : Jan. 10, 2014
    Description :  example use of SAT_AppStorage emulator (Serial version)
                   
    Usage :        - same as SAT_AppStorage

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
#include <EEPROM.h>
#include <I2C_add.h>
#include <nanosat_message.h>
#include <SAT_AppStorageEMU.h>


SAT_AppStorageEMU store;    // instance of the class


// NOTE : in this example, all data are stored into a data structure
// and sent as a block through SAT_AppStorage
struct dataStruct_ {
  unsigned long int ms;
  // put everything you'd like to write ?
  float f;
  int i;
  byte b;
} data;


void setup() {
  // initialization of the emulator (SD configuration)
  Serial.begin(9600);
  
  store.init( true ); // debug : outputing verbose lines on Serial
}


void loop() {
  data.ms = millis();
  data.f = 0.6;    // just some constant values to demonstrate the example
  data.i = 42;     // should be filled with values coming from sensors ? or anything else ?
  data.b = 0x27;

  byte * t_bytePtr = (byte *)&data;  // gets a byte* pointer from the pointer to the data structure

  store.send(t_bytePtr, 0, sizeof(struct dataStruct_));   // sends data into the communication file and queue for transfer
                                                          // WARNING : introduces a 100ms delay

  delay(1000);
}

