#ifndef BTN
#define BTN
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
#endif
