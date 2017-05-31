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

   Copyright 2014 - 2017 Emanuele Caruso. See the LICENSE file for details.
 */
 
#ifndef NKROProtocol_h
#define NKROProtocol_h

#include "Protocol.h"
#include <Keyboard.h>

/**
 * Sends the current chord using the NKRO keyboard emulation.
 */
class NKROProtocol : public Protocol {
public:
  
  NKROProtocol() {
    Keyboard.begin();
  }
  
  void sendChord(const boolean (&currentChord)[ROWS][COLS]) const {
    // QWERTY mapping
    char qwertyMapping[ROWS][COLS] = {
      {'q', 'w', 'e', 'r', 't', ' '},
      {'a', 's', 'd', 'f', 'g', ' '},
      {'c', 'v', 'n', 'm', '3', ' '},
      {'u', 'i', 'o', 'p', '[', ' '},
      {'j', 'k', 'l', ';', '\'', ' '}
    };
    int keyCounter = 0;
    char qwertyKeys[ROWS * COLS];
    boolean firstKeyPressed = false;
  
    // Calculate qwerty keys array using qwertyMappings[][]
    for (int i = 0; i < ROWS; i++) {
      for (int j = 0; j < COLS; j++) {
        if (currentChord[i][j]) {
          qwertyKeys[keyCounter] = qwertyMapping[i][j];
          keyCounter++;
        }
      }
    }
    // Emulate keyboard key presses
    for (int i = 0; i < keyCounter; i++) {
      if (qwertyKeys[i] != ' ') {
        Keyboard.press(qwertyKeys[i]);
        if (!firstKeyPressed) {
          firstKeyPressed = true;
        } else {
          Keyboard.release(qwertyKeys[i]);
        }
      }
    }
    Keyboard.releaseAll();
  }
};

#endif // NKROProtocol_h

