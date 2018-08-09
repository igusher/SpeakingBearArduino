#include "Btn.h"

#ifndef BERLINER_BEAR_BTN
#define BERLINER_BEAR_BTN

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
#endif
