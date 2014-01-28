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

#ifndef _SAMPLINGMETHOD_H_
#define _SAMPLINGMETHOD_H_

#include "MaskHandler.h"
#include "DataSerieProcessor.h"

class SamplingMethod {
  protected:
    DataSerieProcessorInterface * proc;    // the handler to process data from the data serie
    MaskHandler mask;                      // a mask of the size of the data serie to be true/false for each data sample (true = data kept after sampling)

  public:
    void initialize(DataSerieProcessorInterface * proc);  // called BEFORE compress, initializes the internal structures for compression
    virtual void sample(float epsilon);                   // the method called for actually compressing the data with error parameter epsilon
    MaskHandler * getMask();                              // getter: returns the internal mask
    void displayMask();                                   // simply display the current mask
    void reduce();                                        // provokes a reduction of the data serie using the mask as a keep/nokeep for each data sample
    int countKeptValues();                                // counts the number of values kept in the data array
};

#endif /* _SAMPLINGMETHOD_H_ */
