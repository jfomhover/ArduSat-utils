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
