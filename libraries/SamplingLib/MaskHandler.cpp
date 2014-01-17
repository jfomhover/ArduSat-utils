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

/*
    Description :  class to handle a binary mask for an array of data
                   the mask is stored into a byte array
    Last Changed : Jan. 17, 2014
*/

#include <Arduino.h>
#include "MaskHandler.h"

// utility : counts the number of bins up in x
// because it looks classy that way ^^
int pop(byte x)
{
    x = x - ((x >> 1) & 0x55);
    x = (x & 0x33) + ((x >> 2) & 0x33);
    x = (x + (x >> 4)) & 0x0F;
    return x & 0x3F;
}

// *** SAMPLING FLAGS MANAGEMENT ***

// forces all values to NOT be kept
void MaskHandler::keepNoValue() {
  for (int i=0; i<maxAlloc; i++)
    assignArray[i] = 0;
}

// forces all values to be kept (for debug mainly)
void MaskHandler::keepAllValue() {
  for (int i=0; i<maxAlloc; i++)
    assignArray[i] = 0xFF;
}

// forces the value n to be kept
void MaskHandler::keepValue(int n, boolean tick) {
  int rank8 = n >> 3;
  int rank1 = n & 0x07;

  byte t_b = assignArray[rank8];
  if (tick)
    t_b = t_b | (1 << rank1);
  else
    t_b = t_b & (0xFF ^ (1 << rank1));

  assignArray[rank8] = t_b;
};

// returns true if a point in the data array is kept by the sampling method
boolean MaskHandler::isValueKept(int n) {
  byte rank8 = (n & 0xF8) >> 3;
  byte rank1 = n & 0x07;
  boolean t_bool = (assignArray[rank8] >> rank1) & 0x01;
  return(t_bool);
};

// counts the number of values kept in the data array
int MaskHandler::countKeptValues() {
  int t_ret = 0;
  for (int i=0; i<maxAlloc; i++)
    t_ret += pop(assignArray[i]);
    
/*
  for (int i=0; i<maxIndex; i++) {
    if (isValueKept(i))
      t_ret++;
  }
*/

  return(t_ret);
}


// initializes the internal structures for covering a mask of length maxLen
void MaskHandler::initialize(int maxLen) {
  maxAlloc = (maxIndex / 8) + 1;

  if (maxAlloc > MASK_MAX_ALLOCATION) {
    maxAlloc = MASK_MAX_ALLOCATION;
    maxIndex = maxAlloc * 8;
  } else {
    maxIndex = maxLen;
  }
}

// returns the maximum index of the mask
int MaskHandler::getMaxIndex() {
  return(maxIndex);
}

