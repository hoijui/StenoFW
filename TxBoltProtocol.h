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
 
#ifndef TxBoltProtocol_h
#define TxBoltProtocol_h

#include "Protocol.h"

/**
 * Sends the current chord over serial using the TX Bolt protocol.
 * TX Bolt uses a variable length packet.
 * Only those bytes that have active keys are sent.
 * The header bytes indicate which keys are being sent.
 * They must be sent in order.
 * It is a good idea to send a zero after every packet.
 * 00XXXXXX 01XXXXXX 10XXXXXX 110XXXXX
 *   HWPKTS   UE*OAR   GLBPRF    #ZDST
 */
class TxBoltProtocol : public Protocol {
public:

//  TxBoltProtocol() {
//    Serial.begin(9600);
//  }

  void sendChord(const boolean (&currentChord)[ROWS][COLS]) const {
    byte chordBytes[] = {B0, B0, B0, B0, B0};
    int index = 0;
  
    // byte 1
    // S-
    if (currentChord[0][0] || currentChord[1][0]) {
      chordBytes[index] |= B00000001;
    }
    // T-
    if (currentChord[0][1]) {
      chordBytes[index] |= B00000010;
    }
    // K-
    if (currentChord[1][1]) {
      chordBytes[index] |= B00000100;
    }
    // P-
    if (currentChord[0][2]) {
      chordBytes[index] |= B00001000;
    }
    // W-
    if (currentChord[1][2]) {
      chordBytes[index] |= B00010000;
    }
    // H-
    if (currentChord[0][3]) {
      chordBytes[index] |= B00100000;
    }
    // Increment the index if the current byte has any keys set.
    if (chordBytes[index]) {
      index++;
    }
  
    // byte 2
    // R-
    if (currentChord[1][3]) {
      chordBytes[index] |= B01000001;
    }
    // A
    if (currentChord[2][0]) {
      chordBytes[index] |= B01000010;
    }
    // O
    if (currentChord[2][1]) {
      chordBytes[index] |= B01000100;
    }
    // *
    if (currentChord[0][4] || currentChord[1][4]) {
      chordBytes[index] |= B01001000;
    }
    // E
    if (currentChord[2][2]) {
      chordBytes[index] |= B01010000;
    }
    // U
    if (currentChord[2][3]) {
      chordBytes[index] |= B01100000;
    }
    // Increment the index if the current byte has any keys set.
    if (chordBytes[index]) {
      index++;
    }
  
    // byte 3
    // -F
    if (currentChord[3][0]) {
      chordBytes[index] |= B10000001;
    }
    // -R
    if (currentChord[4][0]) {
      chordBytes[index] |= B10000010;
    }
    // -P
    if (currentChord[3][1]) {
      chordBytes[index] |= B10000100;
    }
    // -B
    if (currentChord[4][1]) {
      chordBytes[index] |= B10001000;
    }
    // -L
    if (currentChord[3][2]) {
      chordBytes[index] |= B10010000;
    }
    // -G
    if (currentChord[4][2]) {
      chordBytes[index] |= B10100000;
    }
    // Increment the index if the current byte has any keys set.
    if (chordBytes[index]) {
      index++;
    }
  
    // byte 4
    // -T
    if (currentChord[3][3]) {
      chordBytes[index] |= B11000001;
    }
    // -S
    if (currentChord[4][3]) {
      chordBytes[index] |= B11000010;
    }
    // -D
    if (currentChord[3][4]) {
      chordBytes[index] |= B11000100;
    }
    // -Z
    if (currentChord[4][4]) {
      chordBytes[index] |= B11001000;
    }
    // #
    if (currentChord[2][4]) {
      chordBytes[index] |= B11010000;
    }
    // Increment the index if the current byte has any keys set.
    if (chordBytes[index]) {
      index++;
    }
  
    // Now we have index bytes followed by a zero byte where 0 < index <= 4.
    index++; // Increment index to include the trailing zero byte.
    for (int i = 0; i < index; i++) {
      Serial.write(chordBytes[i]);
    }
  }
};

#endif // TxBoltProtocol_h

