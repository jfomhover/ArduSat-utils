
/*
void printDirectory(File dir, int numTabs) {
   while(true) {
     
     File entry =  dir.openNextFile();
     if (! entry) {
       // no more files
       //Serial.println("**nomorefiles**");
       break;
     }
     for (uint8_t i=0; i<numTabs; i++) {
       Serial.print('\t');
     }
     Serial.print(entry.name());
     if (entry.isDirectory()) {
       Serial.println("/");
//       printDirectory(entry, numTabs+1);
     } else {
       // files have sizes, directories do not
       Serial.print("\t\t");
       Serial.println(entry.size(), DEC);
     }
     entry.close();
   }
};
*/

boolean isFileChar(char c) {
  if (c == '.')
    return(true);
  if ((c >= 'a') && (c <= 'z'))
    return(true);
  if ((c >= 'A') && (c <= 'Z'))
    return(true);
  if ((c >= '0') && (c <= '9'))
    return(true);
  return(false);
};

char filename[16];

char * promptFile() {
//  File root = SD.open("/");
  
//  printDirectory(root, 0);
//  root.close();

  Serial.println("type the name of the file you want to open");
  Serial.print("> ");
  
  for (int i=0; i<16; i++)
    filename[i] = 0x00;
  int t_count = 0;
  
  while (true) {
    if (Serial.available() > 0) {
      filename[t_count] = Serial.read();
      if ((!isFileChar(filename[t_count])) || (t_count >= 15)) {
        delay(50);
        while(Serial.available() > 0) { Serial.read(); };  // purge the input channel
        filename[t_count] = '\0';
        return(filename);
      }
      t_count++;
    }
  }
};

