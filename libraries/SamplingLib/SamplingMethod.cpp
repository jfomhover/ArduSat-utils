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
    Description :  virtual class for operating a sampling method
                   this class is basically useless in itself, and should be implemented by another
    Last Changed : Jan. 17, 2014
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

