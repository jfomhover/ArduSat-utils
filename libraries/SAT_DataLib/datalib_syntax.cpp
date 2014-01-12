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

#include <Arduino.h>
#include "SAT_DataLib.h"
#include "datalib_syntax.h"
#include "datalib_datatypes.h"

uint16_t alldatatypes[DATATYPE_KNOWNCOUNT] = {
  DATATYPE_MS,
  DATATYPE_SAT_LUM1,
  DATATYPE_SAT_LUM2,
  DATATYPE_SAT_MAG,
  DATATYPE_SAT_TMP1,
  DATATYPE_SAT_TMP2,
  DATATYPE_SAT_TMP3,
  DATATYPE_SAT_TMP4,
  DATATYPE_SAT_INFRATHERM,
  DATATYPE_SAT_ACCEL,
  DATATYPE_SAT_GYRO,
  DATATYPE_SAT_GEIGER1,
  DATATYPE_SAT_GEIGER2,
  DATATYPE_CRC16
};

const int alldatatypes_lenghts[DATATYPE_KNOWNCOUNT] = {
  DATATYPE_SIZE_MS,
  DATATYPE_SIZE_SAT_LUM,
  DATATYPE_SIZE_SAT_LUM,
  DATATYPE_SIZE_SAT_MAG,
  DATATYPE_SIZE_SAT_TMP,
  DATATYPE_SIZE_SAT_TMP,
  DATATYPE_SIZE_SAT_TMP,
  DATATYPE_SIZE_SAT_TMP,
  DATATYPE_SIZE_SAT_INFRATHERM,
  DATATYPE_SIZE_SAT_ACCEL,
  DATATYPE_SIZE_SAT_GYRO,
  DATATYPE_SIZE_SAT_GEIGER,
  DATATYPE_SIZE_SAT_GEIGER,
  DATATYPE_SIZE_CRC16
};

int computeChunkSize(uint16_t types) {
  int t_ret = PACKET_SIZE_CHUNKHEADER;
  for (int i=0; i<DATATYPE_KNOWNCOUNT; i++) {
    if (types & alldatatypes[i])
      t_ret += alldatatypes_lenghts[i];
  }
  return(t_ret);
};

int getDatatypeIndex(uint16_t type, uint16_t packettypes) {
  int t_index = PACKET_SIZE_CHUNKHEADER;
  for (int i=0; i<DATATYPE_KNOWNCOUNT; i++) {
    if ((type > alldatatypes[i]) && (packettypes & alldatatypes[i]))
      t_index += alldatatypes_lenghts[i];
  }
  return(t_index);
};

int getDatatypeSize(uint16_t type) {
  for (int i=0; i<DATATYPE_KNOWNCOUNT; i++) {
    if (type == alldatatypes[i])
      return(alldatatypes_lenghts[i]);
  }
  return(0);
};
