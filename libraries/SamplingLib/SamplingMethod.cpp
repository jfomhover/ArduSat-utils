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

    Description :  virtual class for operating a sampling method
                   this class is basically useless in itself, and should be implemented by another
    Last Changed : Jan. 17, 2014

********************************************************************
*/

#include <Arduino.h>
#include "SamplingMethod.h"
#include "MaskHandler.h"
#include "DataSerieProcessor.h"

    // called BEFORE compress, initializes the internal structures for compression
    void SamplingMethod::initialize(DataSerieProcessorInterface * proc) {
      this->proc = proc;
      mask.initialize(proc->getLength());
    };
    
    // the method called for actually compressing the data with error parameter epsilon
    void SamplingMethod::sample(float epsilon) {
      mask.keepAllValue();  // NOTE : does nothing, should be implemented by another specific class
    };
    
    // simply display the current mask
    void SamplingMethod::displayMask() {
      for (int i=0; i<proc->getLength(); i++)
        if (mask.isValueKept(i))
          Serial.print(1);
        else
          Serial.print(0);
        Serial.println();
    };
    
    // provokes a reduction of the data serie using the mask as a keep/nokeep for each data sample
    void SamplingMethod::reduce() {
      proc->reduce(mask);  // simply transfered to the data processor
    };

    // getter for the internal mask (may be used by the user/dev)
    MaskHandler * SamplingMethod::getMask() { return(&mask); };
    
    // counts the number of values kept in the data array
    int SamplingMethod::countKeptValues() {
      return(mask.countKeptValues());
    };

