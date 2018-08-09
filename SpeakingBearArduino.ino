#include<SD.h>
#include<SPI.h>
#include<MemoryFree.h>
#include "Btn.h"
#include "BerlinerBearBtn.h"
#include "Folder.h"
#include "Player.h"
#include "BerlinerBear.h"

const int BTNS = 1;

int cs = 4;

const char * folder1 = "01";
const char * folder2 = "02";
const char * folder3 = "03";

const int WHITE_LED_PIN = 3;
const int RED_LED_PIN = 5;
const int GREEN_LED_PIN = 6;


const int * btnPins = new int[3] { 2, 7, 8};

BerlinerBear * bear;
void setup() {
  initSerial();
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
//  initLED();
  Serial.println("Starting...");
  initSD();
  char * folderNames[3] = {folder1, folder2, folder3};
  bear = new BerlinerBear(btnPins, folderNames, BTNS);
//  success();
  Serial.print("freeMemory()=");
  Serial.println(freeMemory());
}

void initLED() {
  pinMode(WHITE_LED_PIN, OUTPUT);
  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  digitalWrite(WHITE_LED_PIN, HIGH);  
}

void initSerial() {
  Serial.begin(9600);
  while (!Serial) {}
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
