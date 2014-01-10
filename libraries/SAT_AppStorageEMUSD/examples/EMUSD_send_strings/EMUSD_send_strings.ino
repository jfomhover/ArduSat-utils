/*
    File :         EMUSD_send_strings.ino
    Author(s) :    + Jean-Francois Omhover (@jfomhover)
    Last Changed : Jan. 10, 2014
    Description :  example use of SAT_AppStorage emulator (SD version)
                   
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
#include <SAT_AppStorageEMUSD.h>


SAT_AppStorageEMUSD store;    // instance of the class


// NOTE : in this example, only a string of chars is sent back through SAT_AppStorage
char * my_string = "Hello World !";


void setup() {
  // initialization of the emulator (SD configuration)
  Serial.begin(9600);
  
  store.init( true,               // debug mode : outputs strings on Serial
              4,                  // chip select : pin of the chip select on your arduino config (uno ethernet = 4)
              true,               // append : true if data sent is appended at the end of the existing file)
              "datalog.bin" );    // filename : name of the file writen on SD
}


void loop() {
  store.send(my_string);   // sends data into the communication file and queue for transfer
                                                          // WARNING : introduces a 100ms delay

  delay(1000);
}

