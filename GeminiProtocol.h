/*
   StenoFW is a firmware for Stenoboard keyboards.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Copyright 2014 Emanuele Caruso. See the LICENSE file for details.
 */
 
#ifndef GeminiProtocol_h
#define GeminiProtocol_h

#include "Protocol.h"

/**
 * Sends the current chord over serial using the Gemini protocol.
 */
class GeminiProtocol : public Protocol {
public:

//  GeminiProtocol() {
//    Serial.begin(9600);
//  }

  void sendChord(const boolean (&currentChord)[ROWS][COLS]) const {
    // Initialize chord bytes
    byte chordBytes[] = {B10000000, B0, B0, B0, B0, B0};
  
    // Byte 0
    if (currentChord[2][4]) {
      chordBytes[0] = B10000001;
    }
  
    // Byte 1
    if (currentChord[0][0] || currentChord[1][0]) {
      chordBytes[1] += B01000000;
    }
    if (currentChord[0][1]) {
      chordBytes[1] += B00010000;
    }
    if (currentChord[1][1]) {
      chordBytes[1] += B00001000;
    }
    if (currentChord[0][2]) {
      chordBytes[1] += B00000100;
    }
    if (currentChord[1][2]) {
      chordBytes[1] += B00000010;
    }
    if (currentChord[0][3]) {
      chordBytes[1] += B00000001;
    }
  
    // Byte 2
    if (currentChord[1][3]) {
      chordBytes[2] += B01000000;
    }
    if (currentChord[2][0]) {
      chordBytes[2] += B00100000;
    }
    if (currentChord[2][1]) {
      chordBytes[2] += B00010000;
    }
    if (currentChord[0][4] || currentChord[1][4]) {
      chordBytes[2] += B00001000;
    }
  
    // Byte 3
    if (currentChord[2][2]) {
      chordBytes[3] += B00001000;
    }
    if (currentChord[2][3]) {
      chordBytes[3] += B00000100;
    }
    if (currentChord[3][0]) {
      chordBytes[3] += B00000010;
    }
    if (currentChord[4][0]) {
      chordBytes[3] += B00000001;
    }
  
    // Byte 4
    if (currentChord[3][1]) {
      chordBytes[4] += B01000000;
    }
    if (currentChord[4][1]) {
      chordBytes[4] += B00100000;
    }
    if (currentChord[3][2]) {
      chordBytes[4] += B00010000;
    }
    if (currentChord[4][2]) {
      chordBytes[4] += B00001000;
    }
    if (currentChord[3][3]) {
      chordBytes[4] += B00000100;
    }
    if (currentChord[4][3]) {
      chordBytes[4] += B00000010;
    }
    if (currentChord[3][4]) {
      chordBytes[4] += B00000001;
    }
  
    // Byte 5
    if (currentChord[4][4]) {
      chordBytes[5] += B00000001;
    }
  
    // Send chord bytes over serial
    for (int i = 0; i < 6; i++) {
      Serial.write(chordBytes[i]);
    }
  }
};

#endif // GeminiProtocol_h

