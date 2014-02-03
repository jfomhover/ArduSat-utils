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
*/

#ifndef _SPECIALPRINT_H
#define _SPECIALPRINT_H

boolean isAlphaNum(byte b) {
  if ((b >= 33) && (b <= 127))
    return(true);
  return(false);
/*  if ((b >= 'A') && (b <= 'Z'))
    return(true);
  if ((b >= 'a') && (b <= 'z'))
    return(true);
  if ((b >= '0') && (b <= '9'))
    return(true);
  */ 
};

void specialPrintByte(byte inByte, Stream * str) {
    if (inByte < 0x10)
      str->print('0');
    str->print(inByte, HEX);
    str->print(' ');
};    

void specialPrintChar(byte inByte, Stream * str) {
    if (isAlphaNum(inByte))
      str->write(inByte);
    else
      str->print(' ');
};

void specialPrintBuffer(char * prefix, byte * buffer, int bufferLen, Stream * str) {
      int byteCount = 0;
      str->print(prefix);
      str->print("[hex]:\t");
      for (int i=0; i<bufferLen; i++) {
        specialPrintByte(buffer[i], str);
        byteCount++;
        if (byteCount > 15) {
          str->print('\n');
          str->print(prefix);
          str->print("[hex]:\t");
          byteCount=0;
        }
      }

      str->print('\n');
      str->print(prefix);
      str->print("[asc]:\t");
      byteCount = 0;
      for (int i=0; i<bufferLen; i++) {
        specialPrintChar(buffer[i], str);
        byteCount++;
        if (byteCount > 47) {
          str->print('\n');
          str->print(prefix);
          str->print("[asc]:\t");
          byteCount=0;
        }
      }
      str->print('\n');
};

#endif /* _SPECIALPRINT_H */
