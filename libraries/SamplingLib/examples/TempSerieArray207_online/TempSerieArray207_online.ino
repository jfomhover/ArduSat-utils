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

    Description :  example of the use of the SamplingLibrary on a dataset of 207 temperature values (see dataSample.h)
                   in this example, the values are previously stored into an array
    Last Changed : Jan. 27, 2014

********************************************************************
*/


#include <Arduino.h>
#include <SampleLib.h>
#include "OnlineSumSampling.h"

#include "dataSample.h"  // see the file


OnlineSumSampling osm;

// the setup routine runs once when you press reset:
void setup() {                
  // display messages on serial port
  Serial.begin(9600);
  
  osm.init(5);
  // here, we prepare a "fake" array of indexes for the data serie
  for (int i=0; i<valuesFloatCount; i++) {
    osm.push((float)i, (float)valuesFloat[i]);
    if (osm.hasPop()) {
      Serial.print(osm.popKey());
      Serial.print('\t');
      Serial.print(osm.popValue());
      Serial.print('\n');
    }
  }
}

// the loop routine runs over and over again forever:
void loop() {
  // nothing to do here
}

