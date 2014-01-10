/*
    File :         SAT_AppStorageEMU.h
    Author(s) :    + Jean-Francois Omhover (@jfomhover)
                   + NanoSatisfi Inc.
    Last Changed : Aug. 8th 2013
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

#ifndef SAT_APP_STORAGE_EMU_H
#define SAT_APP_STORAGE_EMU_H

#include <inttypes.h>
#include <stdlib.h>
#include <Arduino.h>

#include <nanosat_message.h>
#include <OnboardCommLayer.h>

class SAT_AppStorageEMU
{
  private:
    int dataCount_;
    uint8_t nodeAddress_;
    boolean debugMode_;

    /*
     * Takes a fixed size of data to be packed into a Nanosat message struct
     * then queued in I2C.
     *
     * @param data    A byte array of data.
     * @param offset  starting offset.
     * @param length  how many bytes to be copied.
     */
    void copyAndSend(byte data[], unsigned int offset, unsigned int length);
  public:
    /*
     * Constructor
     */
    SAT_AppStorageEMU();
    void init(boolean debug); // is the class outputing verbose lines on Serial

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
