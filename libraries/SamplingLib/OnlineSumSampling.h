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

Description :  virtual class for operating an online sampling method
this class is basically useless in itself, and should be implemented by another

Last Changed : Feb. 7, 2014

********************************************************************
*/

#ifndef _ONLINESAMPLINGMETHOD_H_
#define _ONLINESAMPLINGMETHOD_H_

#include "MaskHandler.h"
#include "DataSerieProcessor.h"

class OnlineSumSampling {
  protected:
    float _epsilon;
    int count;
    float ta,a;
    float tb,b;
    float _epsilonLimit;
    float tmark, mark;
    boolean _hasPop;

  public:
    void init(float epsilonlimit) {
    	_epsilonLimit = epsilonlimit;
    	_epsilon = 0;
    	count = 0;
    }
    void push(float key, float val) {
    	if (count == 0) {
    		ta = key;
    		a = val;
    		_hasPop = true;
    		tmark = ta;
    		mark = a;
    		count++;
    		return;
    		_epsilon = 0;
    	}
    	if (count == 1) {
    		tb = key;
    		b = val;
    		count++;
    		return;
    		_epsilon = 0;
    	}
    	float lambda = (tb-ta)/(key-ta);
    	float b_approx = (1-lambda)*a + lambda*val;
    	float t_err = abs(b_approx-b) + _epsilon;

    	if (t_err > _epsilonLimit) {
    		markStep();
    		ta = tb;
    		a = b;
    		tb = key;
    		b = val;
    		count = 2;
    		_epsilon = 0;
    		return;
    	}
    	_epsilon = t_err;
    	tb = key;
    	b = val;
    	count++;
    }
    void markStep() {
    	_hasPop = true;
    	tmark = tb;
    	mark = b;
    }
    boolean hasPop() {
    	return(_hasPop);
    }
    float popKey() {
    	return(tmark);
    }
    float popValue() {
    	_hasPop = false;
    	return(mark);
    }
    float getCurrentEpsilon() { return(_epsilon); };
    float getEpsilonLimit() { return(_epsilonLimit); };
    void setEpsilonLimit(float epsilon) { _epsilonLimit = epsilon; };
};

#endif /* _ONLINESAMPLINGMETHOD_H_ */
