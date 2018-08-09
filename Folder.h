#ifndef FOLDER
#define FOLDER

#include <SD.h>
#include <SPI.h>
#include <MemoryFree.h>
#include "BerlinerBearBtn.h"

class Folder {
  public:
    Folder (const char * _folder) {
      this -> name = _folder;
      this -> dir = SD.open(_folder);
      this -> count = 0;
      File nextFile = dir.openNextFile();
      Serial.println(_folder);
      while (nextFile != 0) {
        this -> count ++;
        Serial.println(nextFile.name());
        nextFile.close();
        nextFile = dir.openNextFile();
      }
    }
    void info();
    char * getFile();

  private:
    char * name;
    File dir;
    int count;
};

void Folder::info() {
  Serial.print(this -> name);
  Serial.print("  ");
  Serial.println(this -> count);
  for (int i = 0; i < this -> count; i++) {
    char * name = this -> getFile();
    Serial.print(i + 1); Serial.print(":  "); Serial.println(name);
    delete [] name;
  }
}

char * Folder::getFile() {
  File nextFile = this -> dir.openNextFile();
  if (nextFile == 0) {
    Serial.println("                next file == 0");
    Serial.print("                "); Serial.print(this -> name);
    this -> dir.close();
    this -> dir = SD.open(this -> name);
    nextFile = dir.openNextFile();
    Serial.print("                "); Serial.println(this -> dir);

  }

  if (nextFile == 0) {
    return 0;
  }

  int dN = strlen(this -> name);

  char * fileName = new char[dN + 1 + strlen(nextFile.name()) + 1];
  strcpy(fileName, this -> name);
  fileName[dN] = '/';
  strcpy(fileName + dN + 1, nextFile.name());
  nextFile.close();
  return fileName;
}

#endif
