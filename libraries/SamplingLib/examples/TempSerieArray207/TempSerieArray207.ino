/*
    Author :       Jean-Francois Omhover (jf.omhover@gmail.com, twitter:@jfomhover)
    URL :          https://github.com/jfomhover/ArduSat-utils
    Description :  example of the use of the SamplingLibrary on a dataset of 207 temperature values (see dataSample.h)
                   in this example, the values are previously stored into an array
    Last Changed : Jan. 17, 2014
    
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

#include <SampleLib.h>
#include <IncSamplingMethod.h>
#include <DataSerieProcessor.h>

#include "dataSample.h"  // see the file

byte indexArray[207];    // array for the indexes of the points

                         // below, the class to manipulate indexes for processing in the sampling method (utility class)
DataSerieProcessor<byte, float> dsp(indexArray,         // array for the indexes
                                    valuesFloat,        // array for the values
                                    valuesFloatCount,   // number of values already stored into the array
                                    valuesFloatCount);  // maximum number of values that can be stored into the array
                                                        // NOTE : here, the data is already acquired, so the last 2 parameters are equal, we won't add new data at the end of the serie

IncSamplingMethod ism;   // the class of the sampling algorithm

// the setup routine runs once when you press reset:
void setup() {                
  // display messages on serial port
  Serial.begin(115200);
  
  // here, we prepare a "fake" array of indexes for the data serie
  for (byte i=0; i<valuesFloatCount; i++)
    indexArray[i] = i;   // each data point of the array valuesFloat is indexes by its position in the array

  dsp.display();         // displays the serie via Serial for checking data BEFORE sampling

  ism.initialize(&dsp);  // initialize the data structures for processing dsp

  unsigned long int ms_before = millis();
  ism.sample(5.0);       // operates the sampling method with error parameter 5.0
                         // NOTE : on this example, it means 5*0.0625 degrees so 0.31°C of error
  unsigned long int ms_after = millis();

  Serial.print("computing time(ms): ");
  Serial.println(ms_after-ms_before);

  Serial.print("count kept values: ");  
  Serial.println(ism.countKeptValues());

  Serial.print("mask: ");  
  ism.displayMask();     // displays the mask via Serial


  Serial.println("reducing...");  
  ism.reduce();          // reduces the data set by removing points that are not kept in the mask (0 = not kept, 1 = kept)
                         // WARNING : modifies the data array definitively !!!

  dsp.display();         // displays the serie via Serial for checking data AFTER sampling
}

// the loop routine runs over and over again forever:
void loop() {
  // nothing to do here
}

