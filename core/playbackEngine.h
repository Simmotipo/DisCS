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
        if (line[0] == 1 && inLoop == 0) {
          loopedFrom = pointerLoc;
          pointerLoc = line[1];
          inLoop = 1;
        }
        else if (line[0] == 1 && inLoop == 1 && loopedFrom == pointerLoc) {
          inLoop = 0;
          pointerLoc++;
        }
        else if (line[0] == 2 && inLoop == 1) {
          if (line[2] == 0) {
            pointerLoc = line[1];
          }
          else if (line[2] != 0 && line[2] == loopedFrom) {
            pointerLoc == line[1];
            pointerLoc++;
          }
        }
        else if (line[0] == 3 && inLoop == 1) {
          if (line[2] == 0) {
            pointerLoc = len;
          }
          else if (line[2] != 0 && line[2] == loopedFrom) {
            pointerLoc == line[1];
          } else {
            pointerLoc++;
          }
        }
        else if (line[0] == 4) { pointerLoc = len; }
        else { pointerLoc++; }
      }
    }
  }
  pointerLoc = oldLoc;
}

void setPointer(int loc) {
  pointerLoc = loc;
}