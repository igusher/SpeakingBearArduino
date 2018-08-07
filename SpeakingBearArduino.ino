 #include<SD.h>
#include<SPI.h>
#include<MemoryFree.h>
#include "TMRpcm.h"
const int BTNS = 1;

class Btn {
  public:
    Btn(int _pin): pin(_pin) {
        pinMode(pin, INPUT);  
    };
    bool isClicked();
    virtual int onClick() = 0;

  protected:
    int pin;
  private:
    int curState = HIGH;
};

bool Btn::isClicked() {
  int newState = digitalRead(this->pin);
  bool res = curState == LOW && newState == HIGH;
  this -> curState = newState;
  return res;
}

class BerlinerBearBtn: public Btn {
  public:
    BerlinerBearBtn(int pin): Btn(pin) {
      this->count = 0;
    };
    int onClick();

  private:
    byte count;
};


int BerlinerBearBtn::onClick() {
  int res = this -> count ++;
  return res;
}


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
        nextFile = dir.openNextFile();
      }

      for (int i = 0 ; i < 100; i++) {
            Serial.print("freeMemory()=");
    Serial.println(freeMemory());
        nextFile = dir.openNextFile();
         if (nextFile == 0) {
          this->dir.rewindDirectory();
          nextFile = dir.openNextFile();
         }
         Serial.println(nextFile.name());
//        char * fn = this -> getFile();
//        Serial.println(fn);
        delay(300);
        nextFile.close();
//        delete [] fn;
      }
      
      Serial.println("DONE");
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
  
  return fileName;
}


TMRpcm tmrpcm;

void play(char * file) {
  Serial.println("PLAY,,,   ");
  Serial.println(file);
  Serial.println(SD.exists(file));

  if (tmrpcm.wavInfo(file)) {
    tmrpcm.play(file);
  } else {
    Serial.println("FATAL ERROR!!!");
  }
}

class BerlinerBearStateMachine {
  public:
    BerlinerBearStateMachine(int * pins, char ** folders) {
      this -> btns = new BerlinerBearBtn*[BTNS];
      this -> folders = new Folder* [BTNS];
      for (int i = 0 ; i < BTNS; i++) {
        this -> btns[i] = new BerlinerBearBtn(pins[i]);
        this -> folders[i] = new Folder(folders[i]);
        this -> folders[i] -> info();
      }
    }
    void loop();
  private:
    void click(int i);
    Folder ** folders;
    BerlinerBearBtn ** btns;
    bool isPlaying = false;

};


void BerlinerBearStateMachine::click(int i) {
  this -> btns[i] -> onClick();
  char * nextFile = this -> folders[i] -> getFile();
//  play(nextFile);
//  delete [] nextFile;
  
}

void BerlinerBearStateMachine::loop() {
  if (tmrpcm.isPlaying()) {
    return;
  }
  for (int i = 0 ; i < BTNS; i++) {
    if (this -> btns[i] -> isClicked()) {
      this -> click(i);
      break;
    }  
  }  
}

int cs = 4;

const char * folder1 = "01";
const char * folder2 = "02";
const char * folder3 = "03";
const char * folder4 = "04";
const int WHITE_LED_PIN = 3;
const int RED_LED_PIN = 5;
const int GREEN_LED_PIN = 6;


const int * btnPins = new int[3] { 2, 7, 8};

BerlinerBearStateMachine * bear;
Folder * f1 = 0;
void setup() {
  pinMode(WHITE_LED_PIN,OUTPUT);  
  pinMode(RED_LED_PIN,OUTPUT);  
  pinMode(GREEN_LED_PIN,OUTPUT);  
  digitalWrite(WHITE_LED_PIN, HIGH);

  initSerial();
  Serial.println("Starting...");
  initSD();  
  initTMRpcm();
  delay(100);
  Serial.println("SETUP finished");

  char * folderNames[4] = {folder1, folder2, folder3, folder4};
  bear = new BerlinerBearStateMachine(btnPins, folderNames); 
  fail();
  success();
}

void initSerial() {
  Serial.begin(9600);
  while (!Serial) {}  
}

void initTMRpcm() {
  tmrpcm.speakerPin = 9;
  tmrpcm.setVolume(6);
}

void initSD() {
  pinMode(cs, OUTPUT);
  pinMode(SS, OUTPUT);
  while (!SD.begin(cs)) {
    Serial.println("SD not ready");
  }
  Serial.println("SD is ready!!!");
}

void loop() {
  bear -> loop();
}


void success() {
  digitalWrite(GREEN_LED_PIN, HIGH);
  digitalWrite(RED_LED_PIN, LOW);  
  digitalWrite(WHITE_LED_PIN, LOW);  
}
void fail() {
  digitalWrite(GREEN_LED_PIN, LOW);
  digitalWrite(RED_LED_PIN, HIGH);  
  digitalWrite(WHITE_LED_PIN, LOW);  
}
