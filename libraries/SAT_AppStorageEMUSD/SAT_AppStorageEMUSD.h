/*
    File :         SAT_AppStorageEMUSD.h
    Author(s) :    + Jean-Francois Omhover (@jfomhover)
                   + NanoSatisfi Inc.
    Last Changed : Aug. 9th 2013
    Description :  emulator class adapted from the SAT_AppStorage class made by NanoSatisfi for ArduSat
                   outputs the values to Serial + SD file instead of communicating them via I2C
                   
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

#ifndef SAT_APP_STORAGE_EMUSD_H
#define SAT_APP_STORAGE_EMUSD_H

#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>
#include <nanosat_message.h>
#include <OnboardCommLayer.h>

class SAT_AppStorageEMUSD
{
  private:
    int dataCount_;
    uint8_t nodeAddress_;
    boolean SDavailable_;
    boolean debugMode_;
    char filename_[13];

    /*
     * Takes a fixed size of data to be packed into a Nanosat message struct
     * then queued in I2C.
     *
     * @param data    A byte array of data.
     * @param offset  starting offset.
     * @param length  how many bytes to be copied.
     */
    void copyAndSend(byte data[], unsigned int offset, unsigned int length);
    void write2SD(byte data[], unsigned int offset, unsigned int length);

public:
    /*
     * Constructor
     */
    SAT_AppStorageEMUSD();

    void init(  boolean debug, 		// is the class printing verbose lines on Serial
  		int csPin, 		// pin of the chip select on your arduino config (uno ethernet = 4)
    		boolean append, 	// false = erase the file to restart capture, true = append data
    		char * filename);	// name of the file, (if NULL, default "datalog.bin")

    /*
     * Simple way to enqueue data to be published to disk.
     *
     * @note        When formatting data, don't forget to add newline or
     *              carriage returns.
     * @param data  A formatted string that needs to be written to disk for
     *   retreival by ground station.
     */
    void send(char data[]);
    void send(byte *data, unsigned int offset, unsigned int length);
};

#endif /* SAT_APP_STORAGE_EMU_H */
