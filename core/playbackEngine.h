#include "compilationEngine.h"
#define highSpkr 10 // pin no. for high buzzer
#define bassSpkr 10 // pin no. for bass buzzer
int xOver = 499;    // Cross over frequecy between bass and high buzzers
                    // This was to allow for dual buzzer use for better sound quality. Deprecated here

//Play a tone
void playTone(int buzzer, unsigned int note, int len) {
  if (note < 10000 && len < 5000){ tone(buzzer, note, len); delay(len); }
}

//Play the entire song
void playSong(int startPos) {
  //Initiate Variables
  int inLoop = 0;
  int loopedFrom = 0;
  if (startPos == "") { startPos = 0; }
  int oldLoc = pointerLoc;
  pointerLoc = startPos;
  int len = sizeof song / sizeof song[0];
  Serial.println(len);

  while (pointerLoc < len) { //Repeat until we reach the end:
    //set up array for the line
    int line[] = {};
    line[0] = song[pointerLoc][0];
    line[1] = song[pointerLoc][1];
    line[2] = song[pointerLoc][2];
    if (line[0] == "") { break; } else {
      //If line isn't blank...
      if (line[0] == 0 && line[2] < 5000) {
        //If it's a note (OpCode 0) and length is less than 5000ms, play it on the correct speaker depending on it's frequency
        if (line[1] > xOver) { playTone(highSpkr, line[1], line[2]); }
        else { playTone(bassSpkr, line[1], line[2]); }
        delay(10);
        pointerLoc++;
      } else {
        //If it's not a note...
        if (line[0] == 1 && inLoop == 0) {
          //If it's a loop back to command (OpCode 1) and we're not already in a loop, initiate a loop...
          loopedFrom = pointerLoc;
          pointerLoc = line[1];
          inLoop = 1;
        }
        else if (line[0] == 1 && inLoop == 1 && loopedFrom == pointerLoc) {
          //If it's a loop back to command (OpCode 1) but we're already in a loop, if the loop we're in was triggered by this
          //instruction, (i.e. we've reached the end of the loop section), record us leaving the loop
          inLoop = 0;
          pointerLoc++;
        }
        else if (line[0] == 2 && inLoop == 1) {
          //If we have a Jump if in Loop command (OpCode 2) and we're in a loop...
          if (line[2] == 0) {
            //If the optional "loopedFrom" arg isn't set, jump and exit loop.
            pointerLoc = line[1];
            inLoop = 0;
          }
          else if (line[2] != 0 && line[2] == loopedFrom) {
            //If the optional "loopedFrom" arg ist set, jump and exit loop only if we're in the loop the command is referring to.
            pointerLoc == line[1];
            inLoop = 0;
            pointerLoc++;
          }
        }
        else if (line[0] == 3 && inLoop == 1) {
          //If it's an End if In Loop command (OpCode 3) and we're in a loop...
          if (line[2] == 0) {
            //If the optional "loopedFrom" arg isn't set, set pointerLoc to the end/length of the song.
            pointerLoc = len;
          }
          else if (line[2] != 0 && line[2] == loopedFrom) {
            //If the optional "loopedFrom" arg is set, set pointerLoc to the end/length of the song only if we're in the loop the command is referring to.
            pointerLoc == len;
          } else {
            pointerLoc++;
          }
        }
        else if (line[0] == 4) { pointerLoc = len; } //If it's an End Unconditionally command (OpCode 4), set pointerLoc to the end/length of the song
        else { pointerLoc++; } //The OpCode isn't valid, ignore the message and move on.
      }
    }
  }
  pointerLoc = oldLoc; //Set the pointerLoc to the value it was before we started playback so editing can continue where it left off.
}

void setPointer(int loc) {
  pointerLoc = loc; //Set the pointerLoc to the supplied value.
}
