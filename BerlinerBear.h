#ifndef BERLINER_BEAR
#define BERLINER_BEAR

#include<MemoryFree.h>
#include "Btn.h"
#include "BerlinerBearBtn.h"
#include "Folder.h"
#include "Player.h"


class BerlinerBear {
  public:
    BerlinerBear(int * pins, char ** folders, int BTNS) {
      this -> BTNS = BTNS;
      this -> player = new Player();
      this -> btns = new BerlinerBearBtn*[BTNS];
      this -> folders = new Folder* [BTNS];
      for (int i = 0 ; i < BTNS; i++) {
        this -> btns[i] = new BerlinerBearBtn(pins[i]);
        this -> folders[i] = new Folder(folders[i]);
        //this -> folders[i] -> info();
      }
      //this -> player -> setVol(5);
    }
    void loop();
  private:
    void click(int i);
    Folder ** folders;
    BerlinerBearBtn ** btns;
    bool isPlaying = false;
    Player * player;
    int BTNS;
};


void BerlinerBear::click(int i) {
  this -> btns[i] -> onClick();
  char * nextFile = this -> folders[i] -> getFile();
  Serial.println(nextFile);
  this -> player -> setVol(5);
  this -> player -> play(nextFile);
    Serial.print("freeMemory()=");
      Serial.println(freeMemory());
  //delete [] nextFile;

}

void BerlinerBear::loop() {
  if (this -> player -> isPlaying()) {
    return;
  }
  for (int i = 0 ; i < this -> BTNS; i++) {
    if (this -> btns[i] -> isClicked()) {
      this -> click(i);
      break;
    }
  }
}
#endif
