/*
    Author :       Jean-Francois Omhover (@jfomhover)
    Last Changed : Jan. 12, 2013
    Description :  library for handling, formatting and decoding data from the ArduSat sensors.
                   it has been thought as a utility for sending packets through the SAT_AppStorage library

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

#ifndef _DATALIB_DATATYPES_H_
#define _DATALIB_DATATYPES_H_

#define DATATYPE_KNOWNCOUNT      14

#define DATATYPE_MS              0x0001
#define DATATYPE_SAT_LUM1        0x0002
#define DATATYPE_SAT_LUM2        0x0004
#define DATATYPE_SAT_MAG         0x0008
#define DATATYPE_SAT_TMP1        0x0010
#define DATATYPE_SAT_TMP2        0x0020
#define DATATYPE_SAT_TMP3        0x0040
#define DATATYPE_SAT_TMP4        0x0080
#define DATATYPE_SAT_INFRATHERM  0x0100
#define DATATYPE_SAT_ACCEL       0x0200
#define DATATYPE_SAT_GYRO        0x0400
#define DATATYPE_SAT_GEIGER1     0x0800
#define DATATYPE_SAT_GEIGER2     0x1000
#define DATATYPE_CRC16           0x8000

#define DATATYPE_SIZE_MS              4
#define DATATYPE_SIZE_SAT_LUM         2
#define DATATYPE_SIZE_SAT_MAG         6
#define DATATYPE_SIZE_SAT_TMP         2
#define DATATYPE_SIZE_SAT_INFRATHERM  2
#define DATATYPE_SIZE_SAT_ACCEL       6
#define DATATYPE_SIZE_SAT_GYRO        6
#define DATATYPE_SIZE_SAT_GEIGER      0
#define DATATYPE_SIZE_CRC16           2

#endif /* _DATALIB_DATATYPES_H_ */
