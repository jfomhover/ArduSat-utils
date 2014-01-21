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

#ifndef _SYNTAX_STRUCTS_H
#define _SYNTAX_STRUCTS_H

struct _Usyntax_captureInfo {
  byte header;  //U
  byte cmd;     //R
  byte camid;
  uint32_t snapshotsize;
  uint16_t datapackagecount;
  byte footer;  // #
};

struct _Usyntax_getpackage {
  byte header;  // U
  byte cmd;     // E
  byte camid;
  uint16_t packageid;
  byte footer;  // '#'
};

struct _Usyntax_receivepackageheader {
  byte header;  // U
  byte cmd;     // F
  byte camid;
  uint16_t packageid;
  uint16_t packagesize;
};

#endif /* _SYNTAX_STRUCTS_H */
