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

const int chipSelect = 4;
byte buffer[40];

// use the syntax beloc to declare the datatypes you'll need
DataChunkDecoder decoder;


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

  Serial.print("Initializing SD card...");
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  pinMode(10, OUTPUT);
  
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open("datachk.bin");

  // if the file is available, write to it:
  if (dataFile) {
    decoder.parseFile(dataFile, buffer, 40);
    dataFile.close();
  }  
  // if the file isn't open, pop up an error:
  else {
    Serial.println("error opening datachk.bin");
  } 
  
}


void loop() {
  // put your main code here, to run repeatedly: 
}
