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

Description :  library for handling, formatting and decoding data from the ArduSat sensors.
it has been thought as a utility for sending packets through the SAT_AppStorage library

********************************************************************
*/

#ifndef _DATALIB_DATATYPES_H_
#define _DATALIB_DATATYPES_H_

#define DATATYPE_KNOWNCOUNT      16

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
#define DATATYPE_USERDEFINED1    0x2000
#define DATATYPE_USERDEFINED2    0x4000
#define DATATYPE_CRC16           0x8000

#define DATATYPE_SIZE_MS              4
#define DATATYPE_SIZE_SAT_LUM         4
#define DATATYPE_SIZE_SAT_MAG         6
#define DATATYPE_SIZE_SAT_TMP         2
#define DATATYPE_SIZE_SAT_INFRATHERM  2
#define DATATYPE_SIZE_SAT_ACCEL       6
#define DATATYPE_SIZE_SAT_GYRO        6
#define DATATYPE_SIZE_SAT_GEIGER      0
#define DATATYPE_SIZE_USERDEFINED     5
#define DATATYPE_SIZE_CRC16           2

#endif /* _DATALIB_DATATYPES_H_ */
