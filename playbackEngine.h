#include "frequencies.h"
#include "compilationEngine.h"
#define highSpkr 10 // pin no. for high speaker
#define bassSpkr 10 // pin no. for bass speaker
int xOver = 499;


void Tone(int buzzer, unsigned int note, int len) {
  if (note < 10000 && len < 5000){ tone(buzzer, note, len); }
}

void playSong(int startPos) {
  int inLoop = 0;
  int loopedFrom = 0;
  if (startPos == "") { startPos = 0; }
  int oldLoc = pointerLoc;
  pointerLoc = startPos;
  int len = sizeof song / sizeof song[0];
  Serial.println(len);
  while (pointerLoc < len) {
    int[] line = {};
    line[0] = song[pointerLoc][0];
    line[1] = song[pointerLoc][1];
    line[2] = song[pointerLoc][2];
    if (line[0] == "") { break; } else {
      if (line[0] == 0 && line[2] < 5000) {
        if (line[1] > xOver) { playTone(highSpkr, line[1], line[2]); }
        else { playTone(playTone(bassSpkr, line[1], line[2])); }
        delay(10);
        pointerLoc++;
      } else {
        //Harry's stuff here
      }
    }
  }
  pointerLoc = oldLoc;
}

void setPointer(int loc) {
  pointerLoc = loc;
}
