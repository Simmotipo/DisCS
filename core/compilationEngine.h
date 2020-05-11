//Initialise array that will contain the song info and the pointer-location variable
int song[512][3] = {};
int pointerLoc = 0;

//Void used to append note. Deprecated and superceeded by addEntry()
void addNote(unsigned int note, int len) {
  Serial.println(pointerLoc);
  song[pointerLoc][0] = 0;
  song[pointerLoc][1] = note;
  song[pointerLoc][2] = len;
  //Serial.println(song[pointerLoc][0] + " " + song[pointerLoc][1] + " " + song[pointerLoc][2]);
  pointerLoc = pointerLoc + 1;
}


//Add info at the location of the pointer, where int code is the OpCode, and int arg1 and int arg2 are the additional arguments.
//Refer to info below void for OpCode and Arg values
void addEntry(int code, int arg1, int arg2) {
 Serial.println(pointerLoc);
  //Implement parity checks here
  if (code == 1 && arg1 > pointerLoc) { return; }
  if (code == 2 && arg1 < pointerLoc) { return; }
  
  song[pointerLoc][0] = code;
  song[pointerLoc][1] = arg1;
  song[pointerLoc][2] = arg2;
  //Serial.println(song[pointerLoc][0] + " " + song[pointerLoc][1] + " " + song[pointerLoc][2]);
  pointerLoc = pointerLoc + 1;
}

/*
OpCode  Name                  Syntax
0       Play note             (0, Hz, ms)
1       Loop back to          (1, pos, n/a)
2       Jump if in Loop       (2, pos, optional loopFrom)
3       End if in Loop        (3, optional loopFrom)
4       End unconditionally   (4, n/a, n/a)
*/
