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

    Description :  A sampling method that tries to fit the data serie with linear approximations
                   This linear approximation is sought by splitting at the points that have a maximum error with the approximation.
                   
    Last Changed : Jan. 27, 2014

********************************************************************
*/

#ifndef _MAXSPLITSAMPLINGMETHOD_H_
#define _MAXSPLITSAMPLINGMETHOD_H_

#include <Arduino.h>
#include "SamplingMethod.h"

class MaxSplitSamplingMethod : public SamplingMethod {
  public:
    // the method called for actually compressing the data with error parameter epsilon
    void sample(float epsilon) {
      // initialization step
      mask.keepNoValue();                  // zeroes all flags
      mask.keepValue(0, true);             // keep first data point
      mask.keepValue(proc->getLength()-1, true);  // keep last data point
    
      int rankA = 0;                  // starts with first and last data points
      int rankB = proc->getLength()-1;
    
      while(rankA < (proc->getLength()-1)) {  // loops until we reach the end of the data array
        int rankSplit = 0;
        float t_error = proc->computeMaxErrorByLinearApprox(rankA, rankB, &rankSplit);  // computes the approximation error between the two data points
    
        if (t_error < epsilon) {                     // IF this approximation error is below epsilon
                                                     // => progress in the ranks to the next data segment
          rankA = rankB;                             // we can skip the current segment
    
          for (int i=(rankA+1); i<proc->getLength(); i++) { // and look for the next segment
            if (mask.isValueKept(i)) {
              rankB = i;
              break;
            }
          }                                          // note : on the next iteration, will check the next segment (if it's not already the end of the line)
          
        } else {                                     // ELSE : if approximation error is above epsilon
                                                     // => split ranks
//          Serial.print("Split at ");
//          Serial.println(rankSplit);
          int t_rank = rankSplit;            // divides the segment at the maximum value of Error
          mask.keepValue(t_rank, true);                   // and add the data point in the middle to the sampled data points
          rankB = t_rank;                            // note : on the next iteration, will check the first half of the segment
        }
      };
    };
};

#endif /* _MAXSPLITSAMPLINGMETHOD_H_ */
