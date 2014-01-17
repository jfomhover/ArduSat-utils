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

#ifndef _INCSAMPLINGMETHOD_H_
#define _INCSAMPLINGMETHOD_H_

/*
    Description :  A sampling method that tries to fit the data serie with linear approximations
                   This linear approximation is sought starting from the beginning to the end,
                   and cutting where this approx doesn't meet the error criteria
                   
                   (this method's complexity is O(n^2)... I guess)
    Last Changed : Jan. 17, 2014
*/

#include <Arduino.h>
#include "SamplingMethod.h"

class IncSamplingMethod : public SamplingMethod {
  public:
    // the method called for actually compressing the data with error parameter epsilon
    void sample(float epsilon) {
      // initialization step
      mask.keepNoValue();                  // zeroes all flags
      mask.keepValue(0, true);             // keep first data point

      int rankA = 0;                  // starts with first and last data points
      int rankB = 1;
    
      while((rankA < (proc->getLength()-1)) && (rankB < (proc->getLength()))) {  // loops until we reach the end of the data array
    
        float t_error = proc->computeMaxErrorByLinearApprox(rankA, rankB);  // computes the approximation error between the two data points
    
        if (t_error < epsilon) {                     // IF this approximation error is below epsilon
          rankB++;                                   // => progress in the ranks
        } else {                                     // ELSE : if approximation error is above epsilon
          mask.keepValue(rankB-1, true);             // keep the point just before
          rankA = rankB-1;                           // start over from this point just before
          rankB++;                                   // no need to inspect rankA-rankB since there's no point between them, so skip to the next point
        }
      };

      mask.keepValue(proc->getLength()-1, true);  // keep last data point
    };
};

#endif /* _INCSAMPLINGMETHOD_H_ */
