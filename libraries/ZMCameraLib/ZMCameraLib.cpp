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

#include <Arduino.h>
#include "ZMCameraLib.h"
#include "specialPrint.h"
#include "syntax_structs.h"

char ZMCAMERALIB_fillerSequence[] = "ZZZZZZZZZZZZZZZZMMMMMMMMMMMMMMMM";
byte ZMCAMERALIB_AskVersionSequence[5] = { 'Z', 'M', 0x03, 'Z', '#' };
//byte ZMCAMERALIB_ChangeIDSequence[7] = {'Z', 'M', 0x09, 0x00, 0x00, 'Z', '#'};

    ZMCamera::ZMCamera(Stream * str) {
      this->camStream = str;
      _id = 0xFF;
      bufferLen = 0;
      _debug = false;
      _error = ZM_RETURN_OK;
    };
    void ZMCamera::setDebug(boolean debug) {
      this->_debug = debug;
    };
    boolean ZMCamera::setID(byte id) {
      this->_id = id;
    };      // sets the ID of the camera to be queried
    
    uint16_t ZMCamera::helloCam() {
      sendMessage((byte*)ZMCAMERALIB_fillerSequence,32);
      sendMessage(ZMCAMERALIB_AskVersionSequence, 5);
      uint16_t t_ret = receiveData(100);
      return(t_ret);
    };
    
    boolean ZMCamera::changeBaud(byte baud) {
      byte U_changeBaud[] = { 'U', 'I', _id, baud, '#' }; // TODO : no idea where the ID is
      sendMessage(U_changeBaud, 6);

      uint16_t t_ret = receiveData(4, 100);

      byte OK_changebaud[] = { 'U', 'I', _id, '#' }; // change from 1 to 2
      boolean t_retcheck = bufferFitAnswer(OK_changebaud, 4);
      bufferReset();

      return(t_retcheck);
    };

    boolean ZMCamera::changeID(byte new_id) {
      byte U_changeID[] = { 'U', 'D', _id, new_id, '#' }; // change from 1 to 2      
      sendMessage(U_changeID, 6);
      
      uint16_t t_ret = receiveData(4, 100);

      byte OK_changeID[] = { 'U', 'D', new_id, '#' }; // change from 1 to 2
      boolean t_retcheck = bufferFitAnswer(OK_changeID, 4);
      bufferReset();
      if (t_retcheck)
        _id = new_id;

      return(t_retcheck);
    };

    boolean ZMCamera::changeCompressionRatio(byte ratio) {
      byte U_changeratio[] = { 'U', 'Q', _id, ratio, '#' }; // change from 1 to 2      
      sendMessage(U_changeratio, 6);
      
      uint16_t t_ret = receiveData(4, 100);

      byte OK_changeratio[] = { 'U', 'Q', _id, '#' }; // change from 1 to 2
      boolean t_retcheck = bufferFitAnswer(OK_changeratio, 4);
      bufferReset();

      return(t_retcheck);
    };

    uint32_t _snapshotsize;
    uint16_t _snapshotpackagecount;
    uint16_t _snapshotpackagesize;
    
    boolean ZMCamera::captureImage(byte resolution, uint16_t package_size) {
      byte U_capture[] = { 'U', 'H', _id, resolution, 0x00, 0x00, '#' }; // change from 1 to 2
      memcpy((void*)(U_capture+4), (void*)&package_size, 2);
      
      sendMessage(U_capture, 7);
      
      uint16_t t_ret = receiveData(4, 2000);
      byte OK_capture[] = { 'U', 'H', _id, '#' }; // change from 1 to 2
      boolean t_retcheck = bufferFitAnswer(OK_capture, 4);
      
      if (!t_retcheck) {
        _snapshotsize = 0;
        _snapshotpackagecount = 0;
        _snapshotpackagesize = 0;
        return(false);
      }

      bufferReset();

      t_ret = receiveData(10, 2000);
      
      struct _Usyntax_captureInfo * msg = (struct _Usyntax_captureInfo *)camBuffer;
      _snapshotpackagecount = msg->datapackagecount;
      _snapshotsize = msg->snapshotsize;
      _snapshotpackagesize = package_size;

      bufferReset();
      
      return(true);
    };
    
    uint32_t ZMCamera::getSnapshotSize() {
      return(_snapshotsize);
    };

    uint16_t ZMCamera::getPackageCount() {
      return(_snapshotpackagecount);
    };
    
    uint16_t ZMCamera::getPackage(uint16_t id) {
      if (id > _snapshotpackagecount) {
        _error = ZM_ERROR_PACKAGEOUTOFBOUND;
        return(0);
      }
      
      _error = ZM_RETURN_OK;
      bufferReset();

      struct _Usyntax_getpackage request;
      request.header = 'U';
      request.cmd = 'E';
      request.camid = _id;
      request.packageid = id;
      request.footer = '#';
      sendMessage((byte*)&request, sizeof(struct _Usyntax_getpackage));
      
      uint16_t t_ret = receiveData(4, 50);

      byte OK_getpackage[] = { 'U', 'E', _id, '#' }; // change from 1 to 2
      boolean t_retcheck = bufferFitAnswer(OK_getpackage, 4);
      bufferReset();
      if (!t_retcheck) {
        _error = ZM_ERROR_NACKRECEIVED;
        _snapshotpackatedataptr = NULL;
        _snapshotpackatedatalen = 0;
        return(0);
      }
      
      uint16_t t_expectedlen = sizeof(struct _Usyntax_receivepackageheader)+_snapshotpackagesize+2;
//      int t_expectedlen = _snapshotpackagesize+9; //sizeof(struct _Usyntax_receivepackageheader)+_snapshotpackagesize+2;
      t_ret = receiveData(t_expectedlen, 50);
//      t_ret = receiveData(_snapshotpackagesize+9, 1000);
//      t_ret = receiveData(ZM_BUFFER_SIZE, 50);
      
      struct _Usyntax_receivepackageheader * header = (struct _Usyntax_receivepackageheader *)camBuffer;
      if (_debug) {
        Serial.print("package_id=");
        Serial.print(header->packageid);
        Serial.print(", package_size=");
        Serial.println(header->packagesize);
      }

      // ERROR HANDLING
      if (header->packagesize != (bufferLen - sizeof(struct _Usyntax_receivepackageheader) - 2)) {
        _error = ZM_ERROR_PACKAGESIZEMISMATCH;
        _snapshotpackatedataptr = NULL;
        _snapshotpackatedatalen = 0;
        return(0);
      }
//      if 
      // TODO : checksum verification
      _snapshotpackatedataptr = (byte*)(camBuffer+sizeof(struct _Usyntax_receivepackageheader));
      _snapshotpackatedatalen = header->packagesize;

      return(_snapshotpackatedatalen);
    };
    
    uint16_t ZMCamera::processAllPackagesToFile(Stream * output) {
      uint16_t t_total = 0;
      for (uint16_t t_packid=1; t_packid <= _snapshotpackagecount; t_packid++) {
        Serial.print("get package ");
        Serial.print(t_packid);
        uint16_t t_err = getPackage(t_packid);
        if (t_err == 0)
          return(0);
        Serial.print(" ");
        Serial.println(getPackageLength());
        output->write(getPackagePTR(), getPackageLength());
        output->flush();
        t_total += getPackageLength();
//        t_packid++;
      }
      return(t_total);
    };
    
    byte * ZMCamera::getPackagePTR() {
      return(_snapshotpackatedataptr);
    };
    uint16_t ZMCamera::getPackageLength() {
      return(_snapshotpackatedatalen);
    };
    
/*    boolean setSnapshotSize(uint32_t snapsize, uint16_t datapackages) {
      byte U_setsnapsize[] = { 'U', 'R', _id, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, '#' }; // change from 1 to 2      
      memcpy((void*)(U_setsnapsize+3), (void*)&snapsize, 4);
      memcpy((void*)(U_setsnapsize+7), (void*)&datapackages, 2);
      sendMessage(U_setsnapsize, 10);
      
      int t_ret = receiveData(100);

      byte OK_setsnapsize[] = { 'U', 'R', _id, '#' }; // change from 1 to 2
      boolean t_retcheck = bufferFitAnswer(OK_setsnapsize, 4);
      bufferReset();

      return(t_retcheck);
    };*/
    
    boolean ZMCamera::bufferFitAnswer(byte * answer, int len) {
      boolean t_ret = true;
      
      if (bufferLen < len)
        t_ret = false;
        
      for (int i=0; i<len; i++) {
        if (answer[i] != camBuffer[bufferLen-len+i])
          t_ret = false;
      }
      
      if (_debug) {      
        if (t_ret) Serial.println("ACK");
        else       Serial.println("NACK");
      }
        
      return(t_ret);
    };

    void ZMCamera::sendMessage(byte * msg, int len) {
      if (_debug)
        specialPrintBuffer("SEND", msg, len);
      for (int i=0; i<len; i++)
        camStream->write(msg[i]);
    };
  
    void ZMCamera::bufferReset() {
      bufferLen = 0;
    };
  
    uint16_t ZMCamera::receiveData(int expSize, unsigned long int timeout) {
      unsigned long int timestamp = millis();
      uint16_t t_buflen = bufferLen;

      while (((millis()-timestamp) < timeout) ) {
        while(camStream->available() && ((bufferLen-t_buflen) < expSize)) {
          int inByte = camStream->read();
          camBuffer[bufferLen++] = inByte;
        };
      };
      if (_debug)
        displayBuffer();
      return(bufferLen-t_buflen);
    };
    
    uint16_t ZMCamera::receiveData(unsigned long int timeout) {
      unsigned long int timestamp = millis();
      uint16_t t_buflen = bufferLen;

      while ((millis()-timestamp) < timeout) {
        while(camStream->available()) {
          int inByte = camStream->read();
          camBuffer[bufferLen++] = inByte;
        };
      };
      if (_debug)
        displayBuffer();
      return(bufferLen-t_buflen);
    };

    void ZMCamera::displayBuffer() {
      specialPrintBuffer("CAM", camBuffer, bufferLen);
    };

    int ZMCamera::getErrorCode() { return(_error); };
