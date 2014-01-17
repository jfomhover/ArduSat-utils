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

#ifndef _SAMPLINGMETHOD_H_
#define _SAMPLINGMETHOD_H_

/*
    Description :  virtual class for operating a sampling method
                   this class is basically useless in itself, and should be implemented by another
    Last Changed : Jan. 17, 2014
*/


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
