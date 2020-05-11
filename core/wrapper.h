#include "playbackEngine.h"       // this include contains the include for "compilationEngine.h" thus is it not required here
int wifiStat[] = {0, 0}; //0 = off (led off), 1 = connecting, 2 = connected (led on), 3 = failed (led flashing). 4 = complete [1] refers to LED status
char ssid[] = SECRET_SSID;        // your network SSID (name)
char pass[] = SECRET_PASS;        // your network password (use for WPA, or use as key for WEP)

void processCMD(String cmd[])
{
  //Print ARGS... cmd[0] is the command, cmd[1] and cmd[2] are the args
  Serial.println("ARGS>> " + cmd[0] + "/" + cmd[1] + "/" + cmd[2]); //Print all the args for the command to the console for debugging
  
  //digitalWrite
  if (cmd[0] == "digitalWrite") { //If it's the digitalWrite command... cmd[1] is pin, cmd[2] is value
    if (cmd[2] == "HIGH") {
      digitalWrite(cmd[1].toInt(), HIGH);               // GET /H turns the LED on
    }
    else if (cmd[2] == "LOW") { digitalWrite(cmd[1].toInt(), LOW); }
  }
  
  //pinMode
  else if (cmd[0] == "pinMode") { //If it's the pinMode command... cmd[1] is pin, cmd[2] is value
    if (cmd[2] == "OUTPUT") {
      pinMode(cmd[1].toInt(), OUTPUT);               // GET /H turns the LED on
    }
    else if (cmd[2] == "INPUT") { pinMode(cmd[1].toInt(), INPUT); }
  }

  //wifiStat
  else if (cmd[0] == "status") {
    wifiStat[0] = cmd[1].toInt();
  }
  else if (cmd[0] == "reconnect") {
    //Could be used in the future to allow a reconnection
  }
  else if (cmd[0] == "tone") { //If it's the tone command... cmd[1] is the speaker, cmd[2] is frequency, cmd[3] is length
    if (cmd[3].toInt() > 5000) { cmd[3] = "5000"; }
    if (cmd[1].toInt() < 14) {
      playTone(cmd[1].toInt(), cmd[2].toInt(), cmd[3].toInt());
    }
  }
  else if (cmd[0] == "addNote") { //If it's the addNote command... cmd[1] is frequency, cmd[2] is length
    int len = cmd[2].toInt();
    if (len > 5000) { len = 5000; }
    unsigned int note = cmd[1].toInt();
    addEntry(0, note, len); //Pass to the addEntry void, adding the a 0 as the first command (OpCode 0 for adding a note)
  }
  else if (cmd[0] == "playSong") {
    if (cmd[1] == "") { playSong(0); } else { playSong(cmd[1].toInt()); } //If it's the playSong command... cmd[1] is start location
  }
  else if (cmd[0] == "pointerLoc" || cmd[0] == "setLoc" || cmd[0] == "") //If it's the pointerLoc or setLoc command... cmd[1] is new pointer location
  {
    int pos = cmd[1].toInt();
    if (pos > 511) { pos = 1;}
    setPointer(pos);
  }
  //simplified cmd
  else if (cmd[0] == "playNote") { //playNote is a simplified version of tone... cmd[1] is frequency, cmd[2] is length (unlike tone does not require you to specify a speaker pin
    int len = cmd[2].toInt();
    if (len > 500) { len = 500; }
    unsigned int note = cmd[1].toInt();
    playTone(10, note, len);
  }
  //Looping/Instruction command
  else if (cmd[0] == "addInstruction") {//If it's the addNote command... cmd [1] is the OpCode, cmd[2] is frequency, cmd[3] is length
    int opcode = cmd[1].toInt();
    int tag1 = cmd[2].toInt();
    int tag2 = cmd[3].toInt();
    addEntry(opcode, tag1, tag2); //Pass to the addEntry void
  }
  //Print completion to console for debugging
  Serial.println("Parsed.");
}
