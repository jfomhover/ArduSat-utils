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

#ifndef _DATALIB_SYNTAX_H_
#define _DATALIB_SYNTAX_H_

#include <Arduino.h>
#include "datalib_datatypes.h"

uint16_t getDatatype(int i);
int computeChunkSize(uint16_t types);
int getDatatypeIndex(uint16_t type, uint16_t packettypes);
int getDatatypeSize(uint16_t type);

extern uint16_t alldatatypes[DATATYPE_KNOWNCOUNT];
extern const int alldatatypes_lenghts[DATATYPE_KNOWNCOUNT];

#endif /* _DATALIB_SYNTAX_H_ */
