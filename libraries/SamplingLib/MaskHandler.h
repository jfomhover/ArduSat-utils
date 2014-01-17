/*
    Author :       Jean-Francois Omhover (jf.omhover@gmail.com, twitter:@jfomhover)
    URL :          https://github.com/jfomhover/ArduSat-utils
    
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

#ifndef _MASKHANDLER_H_
#define _MASKHANDLER_H_

/*
    Description :  class to handle a binary mask for an array of data
                   the mask is stored into a byte array
    Last Changed : Jan. 17, 2014
*/


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
