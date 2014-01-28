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

    Description :  Interface class for the serie to be handled by SamplingMethod
                   without knowledge of the inner types of the data seri
    Last Changed : Jan. 27, 2014

********************************************************************
*/

#ifndef _DATASERIEPROCESSOR_H_
#define _DATASERIEPROCESSOR_H_


#include "DataSerieMap.h"


class DataSerieProcessorInterface {
  public:
    virtual int getLength() { return(0); };                                              // returns the length of the data serie
    virtual float variance1(int rankA, int rankB) { return(0.0); };                      // returns the variance of a segment
    virtual float globalVariance() { return(variance1(0,getLength())); };                // returns the variance of the whole serie
    virtual float computeMaxErrorByLinearApprox(int rankA, int rankB, int * rankMax = NULL) { return(0.0); };  // returns the maximum difference between a linear approximation and the data sample
                                                                                         // (rankA and rankB being the extrema of the segments)
                                                                                         // puts the rank of the maximum error at rankMax
    virtual float computeSumErrorByLinearApprox(int rankA, int rankB) { return(0.0); };  // returns the sum of the abs difference between a linear approximation and the data sample
    virtual float computeErrorByLinearApproxAtIndex(int rankA, int rankB, int rankI) { return(0.0); };  // returns the abs difference between the value at rankI and it's linear approx between rankA-rankB
    virtual void reduce(MaskHandler mask) {};                                            // reduce the data serie using the mask (keep only the points that are up in the mask)
};

/*
  Here's the hardcore part, multiple inheritance
  for DataSerieProcessor to be able to handle data AND be manipulated by SamplingMethod
  NOTE : did multiple inheritance just because i didn't want the dev/user
  to have to create systematically two classes (DataSerieMap and DataSerieProcessor)
*/

template <typename T, typename U> class DataSerieProcessor : public DataSerieMap<T,U>, public DataSerieProcessorInterface {
  public:
    // for these constructors, report to DataSerieMap.h
    DataSerieProcessor(int len) : DataSerieMap<T,U>(len), DataSerieProcessorInterface() {};
    DataSerieProcessor() : DataSerieMap<T,U>(), DataSerieProcessorInterface() {};
    DataSerieProcessor(T * indexes, U * values, int curLen, int maxLen) : DataSerieMap<T,U>(indexes, values, curLen, maxLen), DataSerieProcessorInterface() {};

    // implementation of the functions of DataSerieProcessorInterface (see the class above for comments)
    int getLength() { return(this->currentLength); };
    
    float variance1(int rankA, int rankB) {
      U t_moy = 0;
      for (int i=rankA; i<rankB; i++) {
        t_moy += this->getValueAtIndex(i);
      }
      
      t_moy /= (rankB - rankA);
      
      float t_var = 0;
      for (int i=rankA; i<rankB; i++) {
        t_var += abs(t_moy - this->getValueAtIndex(i));  // TODO : norme 2
      }
      return(t_var);
    };
    
    float computeSumErrorByLinearApprox(int rankA, int rankB) {
      if (rankA == rankB)
        return(0.0);
      if (rankA == (rankB-1))
        return(0.0);
      float t_ret = 0;
      for (int i=(rankA+1); i<rankB; i++) {
        float t_localErr = this->computeErrorByLinearApproxAtIndex(rankA, rankB, i);
//        Serial.println(t_localErr);
        t_ret += t_localErr;
      }
      return(t_ret);
    };
/*    
    float computeMaxErrorByLinearApprox(int rankA, int rankB) {
      if (rankA == rankB)
        return(0.0);
      if (rankA == (rankB-1))
        return(0.0);
      float t_ret = 0;
      for (int i=(rankA+1); i<rankB; i++) {
        float t_localErr = this->computeErrorByLinearApproxAtIndex(rankA, rankB, i);
//        Serial.println(t_localErr);
        if (t_localErr > t_ret)
          t_ret = t_localErr;
      }
      return(t_ret);
    };
*/
    float computeMaxErrorByLinearApprox(int rankA, int rankB, int * rankMax = NULL) {
/*      Serial.print("DataSerieProcessor("); 
      Serial.print(rankA);
      Serial.print(',');
      Serial.print(rankB);
      Serial.println(')'); */
      if (rankA == rankB)
        return(0.0);
      if (rankA == (rankB-1))
        return(0.0);
      float t_ret = 0;
      float t_localErrMax = 0;
      int t_localErrMaxRank = (rankB+rankA)/2;
      for (int i=(rankA+1); i<rankB; i++) {
        float t_localErr = this->computeErrorByLinearApproxAtIndex(rankA, rankB, i);
        if (t_localErr > t_localErrMax) {
          t_localErrMax = t_localErr;
          t_localErrMaxRank = i;
        }
//        Serial.println(t_localErr);
        if (t_localErr > t_ret)
          t_ret = t_localErr;
      }
      if (rankMax != NULL)
        *rankMax = t_localErrMaxRank;
      return(t_ret);
    };
    
    float computeErrorByLinearApproxAtIndex(int rankA, int rankB, int rankI) {
//      float lambda = ((float)(rankI - rankA))/((float)(rankB - rankA));
      float lambda = ((float)(this->getKeyAtIndex(rankI) - this->getKeyAtIndex(rankA)))/((float)(this->getKeyAtIndex(rankB) - this->getKeyAtIndex(rankA)));
      float t_approxval = (1-lambda)*(this->getValueAtIndex(rankA))+(lambda)*(this->getValueAtIndex(rankB));
      float t_localErr = abs(this->getValueAtIndex(rankI) - t_approxval);
      return(t_localErr);
    };
    
    void reduce(MaskHandler mask) {
      int t_index = 0;
      int t_len = this->currentLength;
      for (int i=0; i<t_len; i++) {
        if (!mask.isValueKept(i))
          this->remove(this->getKeyAtIndex(t_index));
        else
          t_index++;
      }
    };
};


#endif /* _DATASERIEPROCESSOR_H_ */
