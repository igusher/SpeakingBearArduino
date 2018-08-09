#ifndef PLAYER
#define PLAYER

#include "TMRpcm.h"

class Player {
    public:
     Player () {
        this -> tmrpcm.speakerPin = 9;
        this -> tmrpcm.setVolume(5);
    }
        void setVol(int v);
        void play(char * file);
        bool isPlaying();
    private:
        TMRpcm tmrpcm;
};

/*Player::Player () {
    this -> tmrpcm.speakerPin = 9;
    this -> tmrpcm.setVolume(6);
}*/

void Player::play(char * file) {
  Serial.println("PLAY,,,   ");
  Serial.println(file);

  if (tmrpcm.wavInfo(file)) {
    tmrpcm.play(file);
  } else {
    Serial.println("FATAL ERROR!!!");
  }
}


bool Player::isPlaying() {
    return tmrpcm.isPlaying();
}

void Player::setVol(int v) {
    this -> tmrpcm.setVolume(v);
}
#endif
