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

    Description :  class to handle a binary mask for an array of data
                   the mask is stored into a byte array
    Last Changed : Jan. 17, 2014

********************************************************************
*/

#ifndef _MASKHANDLER_H_
#define _MASKHANDLER_H_


#define MASK_MAX_ALLOCATION  32             // TODO : has a fixed size for now, should be allocated on the fly (malloc)
                                            // NOTE : IMHO, fixed size has its advantage here for simplifying memory consumption
                                            
#include <Arduino.h>

class MaskHandler {
  private:
    byte assignArray[MASK_MAX_ALLOCATION];  // array for keeping track of the data points "kept" by the sampling method
    int maxIndex;                           // maximum possible index in the mask array (0-255)
    int maxAlloc;                           // number of bytes allocated (size of assignArray)

  public:
    void initialize(int maxLen);                  // initializes the internal structures for covering a mask of length maxLen
    int getMaxIndex();                            // returns the maximum index of the mask
    boolean isValueKept(int n);                   // returns true if a point in the data array is kept by the sampling method
    void keepNoValue();                           // forces all values to NOT be kept
    void keepAllValue();                          // forces all values to be kept (for debug mainly)
    void keepValue(int n, boolean tick);          // forces the value n to be kept
    int countKeptValues();                        // counts the number of values kept in the data array
};

#endif /* _MASKHANDLER_H_ */
