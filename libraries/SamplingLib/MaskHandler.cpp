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
    Last Changed : Jan. 27, 2014

********************************************************************
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
/*  for (int i=0; i<maxAlloc; i++)
    t_ret += pop(assignArray[i]);
*/    

  for (int i=0; i<maxIndex; i++) {
    if (isValueKept(i))
      t_ret++;
  }

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

