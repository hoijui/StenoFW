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

   Copyright 2017 Emanuele Caruso. See the LICENSE file for details.
 */

#ifndef TestProtocol_h
#define TestProtocol_h

#include "Protocol.h"
#include <Keyboard.h>

/**
 * Sends the current chord as an ASCII matrix using keyboard emulation.
 *
 * This code allows to send either of two different representations
 * of the keyboard matrix; electronic or haptic.
 * Electronically, the Stenoboard is a (quite dense) 5*6 matrix,
 * with the first two rows being the left hand consonants,
 * the third row being the vowels plus number key,
 * and the last two rows being the right hand consonants.
 * Haptically (as in, what the user sees and touches),
 * the Stenoboard is a (less dense) 4*12 matrix,
 * with the first row being the number bar,
 * thwe second and third row being the function keys
 * and consonants of both hands,
 * and the last row being the vowels.
 */
class TestProtocol : public Protocol {

  /**
   * Whether to send the electronic or the haptic representation
   * of the keyboard matrix.
   */
  const boolean matrixElectronic;

  void sendKeyPress(const char key) const {

    Keyboard.write(key);
    delay(15);
  }

  void sendChordElectronicMatrix(const boolean (&currentChord)[ROWS][COLS]) const {

    // Write column headers
    sendKeyPress(' ');
    sendKeyPress(' ');
    for (int column = 0; column < COLS; column++) {
      sendKeyPress('0' + column);
    }
    sendKeyPress('\n');

    // Write column header separator
    sendKeyPress(' ');
    sendKeyPress(' ');
    for (int column = 0; column < COLS; column++) {
      sendKeyPress('-');
    }
    sendKeyPress('\n');

    // Write the chord matrix
    for (int row = 0; row < ROWS; row++) {
      // Write the row header
      sendKeyPress('0' + row);
      // Write the row header separator
      sendKeyPress('|');

      for (int column = 0; column < COLS; column++) {
        if (currentChord[row][column]) {
          sendKeyPress('X');
        } else {
          sendKeyPress(' ');
        }
      }
      sendKeyPress('\n');
    }
    sendKeyPress('\n');
  }

  void sendChordHapticMatrix(const boolean (&currentChord)[ROWS][COLS]) const {

    sendKeyPress('\n');

    // send number bar
    sendKeyPress('0');
    sendKeyPress('(');
    sendKeyPress('n');
    sendKeyPress('u');
    sendKeyPress('m');
    sendKeyPress(')');
    sendKeyPress('|');
    const char numBarChar = currentChord[2][4] ? 'X' : ' ';
    for (int column = 0; column < 6; column++) {
      sendKeyPress(numBarChar);
    }
    sendKeyPress('|');
    for (int column = 0; column < 6; column++) {
      sendKeyPress(numBarChar);
    }
    sendKeyPress('|');
    sendKeyPress('\n');

    // send consonant keys row
    for (int consRow = 0; consRow < 2; consRow++) {
      sendKeyPress('0' + 1 + consRow);
      sendKeyPress('(');
      sendKeyPress('c');
      sendKeyPress('o');
      sendKeyPress('1' + consRow);
      sendKeyPress(')');
      sendKeyPress('|');
      sendKeyPress('-'); // we can not know if the function key is pressed or not
      for (int column = 0; column < 5; column++) {
        sendKeyPress(currentChord[consRow + 0][column] ? 'X' : ' ');
      }
      sendKeyPress('|');
      sendKeyPress(currentChord[consRow + 0][4] ? 'X' : ' ');
      for (int column = 0; column < 5; column++) {
        sendKeyPress(currentChord[consRow + 3][column] ? 'X' : ' ');
      }
      sendKeyPress('|');
      sendKeyPress('\n');
    }

    // send vowel keys row
    sendKeyPress('3');
    sendKeyPress('(');
    sendKeyPress('v');
    sendKeyPress('o');
    sendKeyPress('w');
    sendKeyPress(')');
    sendKeyPress(' ');
    sendKeyPress(' ');
    sendKeyPress(' ');
    sendKeyPress(' ');
    sendKeyPress('|');
    sendKeyPress(currentChord[2][0] ? 'X' : ' ');
    sendKeyPress(currentChord[2][1] ? 'X' : ' ');
    sendKeyPress('|');
    sendKeyPress(currentChord[2][2] ? 'X' : ' ');
    sendKeyPress(currentChord[2][3] ? 'X' : ' ');
    sendKeyPress('|');
    sendKeyPress('\n');

    sendKeyPress('\n');
  }

public:

  TestProtocol(const boolean matrixElectronic = false)
    : matrixElectronic(matrixElectronic)
  {
    Keyboard.begin();
  }

  virtual void sendChord(const boolean (&currentChord)[ROWS][COLS]) const {

    if (matrixElectronic) {
      sendChordElectronicMatrix(currentChord);
    } else {
      sendChordHapticMatrix(currentChord);
    }
  }
};

#endif // TestProtocol_h

