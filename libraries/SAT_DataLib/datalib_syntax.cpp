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

#include <Arduino.h>
#include "SAT_DataLib.h"
#include "datalib_syntax.h"
#include "datalib_datatypes.h"
#include "datalib_userdefined.h"

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
  DATATYPE_USERDEFINED1,
  DATATYPE_USERDEFINED2,
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
  DATATYPE_SIZE_USERDEFINED,
  DATATYPE_SIZE_USERDEFINED,
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
