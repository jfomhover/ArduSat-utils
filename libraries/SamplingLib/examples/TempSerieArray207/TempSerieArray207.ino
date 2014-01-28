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

// CONFIG : uncomment one of the two lines below
//#define USE_INCSAMPLING
#define USE_MAXSPLITSAMPLING


#include <Arduino.h>
#include <SampleLib.h>
#ifdef USE_INCSAMPLING
#include "IncSamplingMethod.h"
#endif
#ifdef USE_MAXSPLITSAMPLING
#include "MaxSplitSamplingMethod.h"
#endif
#include "DataSerieProcessor.h"

#include "dataSample.h"  // see the file

byte indexArray[207];    // array for the indexes of the points

                         // below, the class to manipulate indexes for processing in the sampling method (utility class)
DataSerieProcessor<byte, float> dsp(indexArray,         // array for the indexes
                                    valuesFloat,        // array for the values
                                    valuesFloatCount,   // number of values already stored into the array
                                    valuesFloatCount);  // maximum number of values that can be stored into the array
                                                        // NOTE : here, the data is already acquired, so the last 2 parameters are equal, we won't add new data at the end of the serie

#ifdef USE_INCSAMPLING
IncSamplingMethod sm;
#endif
#ifdef USE_MAXSPLITSAMPLING
MaxSplitSamplingMethod sm;   // the class of the sampling algorithm
#endif

// the setup routine runs once when you press reset:
void setup() {                
  // display messages on serial port
  Serial.begin(115200);
  
  // here, we prepare a "fake" array of indexes for the data serie
  for (byte i=0; i<valuesFloatCount; i++)
    indexArray[i] = i;   // each data point of the array valuesFloat is indexes by its position in the array

  dsp.display();         // displays the serie via Serial for checking data BEFORE sampling

  sm.initialize(&dsp);  // initialize the data structures for processing dsp

  unsigned long int ms_before = millis();
  sm.sample(5.0);       // operates the sampling method with error parameter 5.0
                         // NOTE : on this example, it means 5*0.0625 degrees so 0.31°C of error
  unsigned long int ms_after = millis();

  Serial.print("computing time(ms): ");
  Serial.println(ms_after-ms_before);

  Serial.print("count kept values: ");  
  Serial.println(sm.countKeptValues());

  Serial.print("mask: ");  
  sm.displayMask();     // displays the mask via Serial


  Serial.println("reducing...");  
  sm.reduce();          // reduces the data set by removing points that are not kept in the mask (0 = not kept, 1 = kept)
                         // WARNING : modifies the data array definitively !!!

  dsp.display();         // displays the serie via Serial for checking data AFTER sampling
}

// the loop routine runs over and over again forever:
void loop() {
  // nothing to do here
}

