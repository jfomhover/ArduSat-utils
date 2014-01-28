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

void specialPrintByte(byte inByte) {
    if (inByte < 0x10)
      Serial.print('0');
    Serial.print(inByte, HEX);
    Serial.print(' ');
};    

void specialPrintChar(byte inByte) {
    if (isAlphaNum(inByte))
      Serial.write(inByte);
    else
      Serial.print(' ');
};

void specialPrintBuffer(char * prefix, byte * buffer, int bufferLen) {
      int byteCount = 0;
      Serial.print(prefix);
      Serial.print("[hex]:\t");
      for (int i=0; i<bufferLen; i++) {
        specialPrintByte(buffer[i]);
        byteCount++;
        if (byteCount > 15) {
          Serial.print('\n');
          Serial.print(prefix);
          Serial.print("[hex]:\t");
          byteCount=0;
        }
      }

      Serial.print('\n');
      Serial.print(prefix);
      Serial.print("[asc]:\t");
      byteCount = 0;
      for (int i=0; i<bufferLen; i++) {
        specialPrintChar(buffer[i]);
        byteCount++;
        if (byteCount > 47) {
          Serial.print('\n');
          Serial.print(prefix);
          Serial.print("[asc]:\t");
          byteCount=0;
        }
      }
      Serial.print('\n');
};

#endif /* _SPECIALPRINT_H */
