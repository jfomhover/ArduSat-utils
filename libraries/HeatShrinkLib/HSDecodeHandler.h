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

    Description :  class that encapsulates the decoder of the heatshrink code
                   from Scott Vokes <scott.vokes@atomicobject.com>
                   available at : https://github.com/atomicobject/heatshrink
                   adapted for Arduino UNO use
    Last Changed : Feb. 2, 2014

********************************************************************
*/

#ifndef HSDECODEHANDLER_H_
#define HSDECODEHANDLER_H_

#include "heatshrink_decoder.h"

//#define HSD_DEBUG_MODE
#define HSD_OUTPUTBUF_SIZE 32

class HSDecodeHandler {
private:
  int compressedDataReceived;
  int uncompressedDataSent;
  byte out_buf[HSD_OUTPUTBUF_SIZE];
  size_t out_size;
  heatshrink_decoder decoder;
  void (*send)(byte*,int);
  
public:
  HSDecodeHandler() {
    out_size = 0;
    compressedDataReceived = 0;
    uncompressedDataSent = 0;
  };
  
  void reset() {
    heatshrink_decoder_reset(&decoder);
  };
  
  void resetOutput() {
    for (int i=0; i<HSD_OUTPUTBUF_SIZE; i++)
      out_buf[i] = 0x00;
  };
  
  void push(byte * buffer, int len) {
#ifdef HSD_DEBUG_MODE
    Serial.print("push(");
    Serial.print(len);
    Serial.println(')');
#endif
    
    compressedDataReceived += len;

    int c_len = 0;
    int c_start = 0;
    while (c_start < len) {
      int c_remain = len-c_start;
      size_t t_size;
      
      HSD_sink_res t_ret;
      t_ret = heatshrink_decoder_sink(&decoder, buffer+c_start, c_remain, &t_size);
#ifdef HSD_DEBUG_MODE
      Serial.print("sink(");
      Serial.print(c_remain);
      Serial.print(',');
      Serial.print(t_size);
      Serial.println(')');
#endif
      
//      if (decoder.state == 1) {
//        HSD_finish_res t_fin;
//        t_fin = heatshrink_decoder_finish(&decoder);

        purgeDecompressOutput();
//        reset();
//      }
      c_start += t_size;
    }
  };
  
  void purgeDecompressOutput() {
    HSD_poll_res t_poll;
    do {
      resetOutput();
      t_poll = heatshrink_decoder_poll(&decoder, out_buf, HSD_OUTPUTBUF_SIZE, &out_size);
#ifdef HSD_DEBUG_MODE
      Serial.print("poll(");
      Serial.print(HSD_OUTPUTBUF_SIZE);
      Serial.print(',');
      Serial.print(out_size);
      Serial.println(')');
#endif
      uncompressedDataSent += out_size;
      this->send(out_buf, out_size);
    } while (t_poll == HSDR_POLL_MORE);
  };
  
  void setupOutputStream(void (*send)(byte*,int)) {
    this->send = send;
  };
  
  void displayStats() {
    Serial.print("recd=");
    Serial.print(compressedDataReceived);
    Serial.print(",sent=");
    Serial.println(uncompressedDataSent);
  };
};

#endif /* HSDECODEHANDLER_H_ */
