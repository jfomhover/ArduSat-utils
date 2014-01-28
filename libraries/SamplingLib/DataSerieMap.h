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

    Description :  Class representing a data serie,
                   basically looks like a bad hashmap but the issue here is to keep alignment
    Last Changed : Jan. 17, 2014

********************************************************************
*/

#ifndef _DATASERIEMAP_H_
#define _DATASERIEMAP_H_


template <typename T, typename U> class DataSerieMap {
  protected:
    T * indexes;        // array for the keys/indexes of the data serie (time ?)
    U * values;         // array for the values indexed into the serie
    int currentLength;  // current length of the data serie
    int maxLength;      // maximum possible length (allocated space)
  
  public:
    // initialization with allocation of maxlen data samples
    DataSerieMap(int maxlen) {
      this->allocate(maxlen);
    };

    // initialization with no allocation of memory space (reported for a later allocate())
    DataSerieMap() {
      indexes = NULL;
      values = NULL;
      currentLength = 0;
      maxLength = 0;
    };

    // initialization with memory space that is previously allocated
    DataSerieMap(T * indexes, U * values, int curLen, int maxLen) {
      this->indexes = indexes;
      this->values = values;
      this->currentLength = curLen;
      this->maxLength = maxLen;
    };

    // allocate memory space for the indexes and values
    int allocate(int maxlen) {
      int t_tsize = maxlen*sizeof(T);
      int t_usize = maxlen*sizeof(U);
      byte * t_ptr = (byte*)malloc(t_tsize+t_usize);
      if (t_ptr == NULL) {
        indexes = NULL;
        values = NULL;
        return(0);
      }
      indexes = (T *)t_ptr;
      values = (U *)(t_ptr+t_tsize);
      currentLength = 0;
      maxLength = maxlen;
      return(maxLength);
    };
    
    // returns the size of the current data serie
    int getSize() {
      return(currentLength);
    };

    // returns the index of the data value associated with key
    int getIndex(T key) {
      for (int i=0; i<currentLength; i++)
        if (indexes[i] == key)
          return(i);
      return(-1);
    };

    // returns the value at index i of the data array
    U getValueAtIndex(int i) {
      return(values[i]); // TODO : error checking
    };

    // returns the key at index i of the data array
    T getKeyAtIndex(int i) {
      return(indexes[i]);
    };

    // checks if the data serie has key
    boolean has(T key) {
      for (int i=0; i<currentLength; i++)
        if (indexes[i] == key)
          return(true);
      return(false);
    };

    // returns the value associated with key (should check has() first)
    U get(T key) {
      for (int i=0; i<currentLength; i++)
        if (indexes[i] == key)
          return(values[i]);
      return(NULL);
    };
    
    // adds (key,value) to the data serie
    boolean push(T key, U value) {
      if (currentLength >= maxLength)
        return(false);
        
      indexes[currentLength] = key;
      values[currentLength] = value;
      currentLength++;
    };

    // removes a data sample with key
    void remove(T key) {
      int t_index = getIndex(key);
      if (t_index < 0)
        return;
      
      memcpy((void*)(indexes+t_index), (void*)(indexes+t_index+1), (currentLength-t_index-1)*sizeof(T));
      memcpy((void*)(values+t_index), (void*)(values+t_index+1), (currentLength-t_index-1)*sizeof(U));
      currentLength--;
    };

    // outputs the data serie on Serial (debug)
    void display() {
      for (int i=0; i<currentLength; i++) {
        Serial.print(indexes[i]);
        Serial.print("=>");
        Serial.println(values[i]);
      };
    };
};

#endif /* _DATASERIEMAP_H_ */

