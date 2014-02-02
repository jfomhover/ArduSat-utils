/*
********************************************************************

Copyright 2014, Jean-François Omhover (jf.omhover@gmail.com) and Nanosatisfi, Inc. (www.nanosatisfi.com)
idea suggested by Pablo Pita, <pablo.pita@gmail.com>
based on the work by Scott Vokes <scott.vokes@atomicobject.com>

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

    Description :  class that encapsulates the encoder of the heatshrink code
                   from Scott Vokes <scott.vokes@atomicobject.com>
                   available at : https://github.com/atomicobject/heatshrink
                   adapted for Arduino UNO use
    Last Changed : Feb. 2, 2014

********************************************************************
*/

#ifndef HSENCODEHANDLER_H_
#define HSENCODEHANDLER_H_

#include "heatshrink_encoder.h"

//#define HSE_DEBUG_MODE
#define HSE_OUTPUTBUF_SIZE 32

void dumphexLINE(byte * data, int len);

class HSEncodeHandler {
public:
  int dataReceived;
  int compressedDataSent;
  byte out_buf[HSE_OUTPUTBUF_SIZE];
  size_t out_size;
  heatshrink_encoder encoder;
  void (*localsend)(byte*,int);
  
public:
  HSEncodeHandler() {
    out_size = 0;
    compressedDataSent = 0;
    dataReceived = 0;
  };
  
  void reset() {
    heatshrink_encoder_reset(&encoder);
  };
  
  void resetOutput() {
    for (int i=0; i<HSE_OUTPUTBUF_SIZE; i++)
      out_buf[i] = 0x00;
    out_size = 0;
  };
  
  void push(byte * buffer, int len) {

#ifdef HSE_DEBUG_MODE
    Serial.print("push(");
    Serial.print(len);
    Serial.print("): ");
    dumphexLINE(buffer, len);
#endif
    dataReceived += len;

    int c_len = 0;
    int c_start = 0;
    while (c_start < len) {
      int c_remain = len-c_start;
      size_t t_size;
      HSE_sink_res t_ret;
      t_ret = heatshrink_encoder_sink(&encoder, buffer+c_start, c_remain, &t_size);
#ifdef HSE_DEBUG_MODE
      Serial.print("sink(");
      Serial.print(c_remain);
      Serial.print(',');
      Serial.print(t_size);
      Serial.print(") :");
      dumphexLINE(buffer+c_start, c_remain);
#endif
      
      if (encoder.state == 1) {

        purgeCompressOutput();
        
      };
      c_start += t_size;
    }
  };
  
  void flush() {
        HSE_finish_res t_fin;
        t_fin = heatshrink_encoder_finish(&encoder);
#ifdef HSE_DEBUG_MODE
      Serial.print("sinfinish(");
      Serial.print(t_fin);
      Serial.println(')');
#endif

    purgeCompressOutput();
    resetOutput();
    reset();
  };
  
  void purgeCompressOutput() {
    resetOutput();
    HSE_poll_res t_poll;
    do {
      t_poll = heatshrink_encoder_poll(&encoder, out_buf, HSE_OUTPUTBUF_SIZE, &out_size);
#ifdef HSE_DEBUG_MODE
      Serial.print("poll(");
      Serial.print(HSE_OUTPUTBUF_SIZE);
      Serial.print(',');
      Serial.print(out_size);
      Serial.print("): ");
      dumphexLINE(out_buf, out_size);
#endif
      compressedDataSent += out_size;
      this->localsend(out_buf, out_size);
    } while (t_poll == HSER_POLL_MORE);
  };

  void setupOutputStream(void (*mysend)(byte*,int)) {
    this->localsend = mysend;
  };
  
  void displayStats() {
    Serial.print("recd\t");
    Serial.print(dataReceived);
    Serial.print("\tsent\t");
    Serial.println(compressedDataSent);
  };
};

void dumphexLINE(byte * data, int len) {
//  return;
  for (int i=0; i<len; i++) {
//      Serial.print("0x");
      if (data[i]<0x10) {Serial.print('0');}
      Serial.print(data[i],HEX);
        Serial.print(" ");
    }
    Serial.println();
}

#endif /* HSENCODEHANDLER_H_ */
