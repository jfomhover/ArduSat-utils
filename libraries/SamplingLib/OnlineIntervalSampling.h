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

class OnlineIntervalSampling {
  protected:
    float _epsilonPlus;
    float _epsilonMinus;
    int count;
    float ta,a;
    float tb,b;
    float _epsilonLimit;
    float tmark, mark;
    boolean _hasPop;

  public:
    void init(float epsilonlimit) {
    	_epsilonLimit = epsilonlimit;
    	_epsilonPlus = 0;
    	_epsilonMinus = 0;
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
    	}
    	if (count == 1) {
    		tb = key;
    		b = val;
    		count++;
    		return;
    	}
    	float tc = key;
    	float c = val;

    	float lambda = (tb-ta)/(key-ta);
    	float b_approx = (1-lambda)*a + lambda*val;


//    	float lambdaB = (tc-ta)/(tb-ta);
//    	float c_approxByB = (1-lambdaB)*a + lambdaB*b;

    	float t_epsPlus,t_epsMinus;

    	if (b_approx > b) {
    		t_epsPlus = _epsilonPlus + b_approx - b;
    		t_epsMinus = _epsilonMinus;
    	}
    	else {
    		t_epsPlus = _epsilonPlus;
    		t_epsMinus = _epsilonMinus + b - b_approx;
    	}

//    	if (t_epsPlus < 0)
//    		t_epsPlus = 0;
//    	if (t_epsMinus < 0)
//    		t_epsMinus = 0;

//    	float t_err = abs(b_approx-b) + _epsilon;
    	float t_err = max(t_epsPlus, t_epsMinus);

    	if (t_err > _epsilonLimit) {
    		markStep();
    		ta = tb;
    		a = b;
    		tb = tc;
    		b = c;
    		count = 2;
    		_epsilonPlus = 0;
    		_epsilonMinus = 0;
    		return;
    	}
    	_epsilonPlus = t_epsPlus;
    	_epsilonMinus = t_epsMinus;
    	tb = tc;
    	b = c;
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
    float getCurrentEpsilon() { return(max(_epsilonPlus,_epsilonMinus)); };
    float getEpsilonLimit() { return(_epsilonLimit); };
    void setEpsilonLimit(float epsilon) { _epsilonLimit = epsilon; };
};

#endif /* _ONLINESAMPLINGMETHOD_H_ */
