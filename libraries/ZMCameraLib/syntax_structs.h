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
