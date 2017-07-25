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

#ifndef StenoKeyboardProtocol_h
#define StenoKeyboardProtocol_h

#include "Protocol.h"

/**
 * Sends the current chord as human readable Steno mnemonic in steno order
 * using keyboard emulation.
 */
class StenoKeyboardProtocol : public Protocol {

  void pressKey(boolean* firstKeyPressed, const char key) const {

    Keyboard.press(key);
    if (*firstKeyPressed) {
      Keyboard.release(key);
    } else {
      (*firstKeyPressed) = true;
    }
  }

public:

  StenoKeyboardProtocol() {
    Keyboard.begin();
  }

  virtual void sendChord(const boolean (&currentChord)[ROWS][COLS]) const {

    boolean firstKeyPressed = false;

    if (currentChord[KEY_SHARP_D0][KEY_SHARP_D1]) {
      pressKey(&firstKeyPressed, '#');
    }

    if (currentChord[KEY_S1_D0][KEY_S1_D1] || currentChord[KEY_S2_D0][KEY_S2_D1]) {
      pressKey(&firstKeyPressed, 'S');
    }
    if (currentChord[KEY_T_D0][KEY_T_D1]) {
      pressKey(&firstKeyPressed, 'T');
    }
    if (currentChord[KEY_K_D0][KEY_K_D1]) {
      pressKey(&firstKeyPressed, 'K');
    }
    if (currentChord[KEY_P_D0][KEY_P_D1]) {
      pressKey(&firstKeyPressed, 'P');
    }
    if (currentChord[KEY_W_D0][KEY_W_D1]) {
      pressKey(&firstKeyPressed, 'W');
    }
    if (currentChord[KEY_H_D0][KEY_H_D1]) {
      pressKey(&firstKeyPressed, 'H');
    }
    if (currentChord[KEY_R_D0][KEY_R_D1]) {
      pressKey(&firstKeyPressed, 'R');
    }

    boolean centerKeyPressed = false;
    if (currentChord[KEY_a_D0][KEY_a_D1]) {
      pressKey(&firstKeyPressed, 'A');
      centerKeyPressed = true;
    }
    if (currentChord[KEY_o_D0][KEY_o_D1]) {
      pressKey(&firstKeyPressed, 'O');
      centerKeyPressed = true;
    }

    if (currentChord[KEY_STAR1_D0][KEY_STAR1_D1] || currentChord[KEY_STAR2_D0][KEY_STAR2_D1]) {
      pressKey(&firstKeyPressed, '*');
      centerKeyPressed = true;
    }

    if (currentChord[KEY_e_D0][KEY_e_D1]) {
      pressKey(&firstKeyPressed, 'E');
      centerKeyPressed = true;
    }
    if (currentChord[KEY_u_D0][KEY_u_D1]) {
      pressKey(&firstKeyPressed, 'U');
      centerKeyPressed = true;
    }

    if (!centerKeyPressed) {
      Keyboard.releaseAll();
      Keyboard.press('-');
      firstKeyPressed = true;
    }

    if (currentChord[KEY_f_D0][KEY_f_D1]) {
      pressKey(&firstKeyPressed, 'F');
    }
    if (currentChord[KEY_r_D0][KEY_r_D1]) {
      pressKey(&firstKeyPressed, 'R');
    }
    if (currentChord[KEY_p_D0][KEY_p_D1]) {
      pressKey(&firstKeyPressed, 'P');
    }
    if (currentChord[KEY_b_D0][KEY_b_D1]) {
      pressKey(&firstKeyPressed, 'B');
    }
    if (currentChord[KEY_l_D0][KEY_l_D1]) {
      pressKey(&firstKeyPressed, 'L');
    }
    if (currentChord[KEY_g_D0][KEY_g_D1]) {
      pressKey(&firstKeyPressed, 'G');
    }
    if (currentChord[KEY_t_D0][KEY_t_D1]) {
      pressKey(&firstKeyPressed, 'T');
    }
    if (currentChord[KEY_s_D0][KEY_s_D1]) {
      pressKey(&firstKeyPressed, 'S');
    }
    if (currentChord[KEY_d_D0][KEY_d_D1]) {
      pressKey(&firstKeyPressed, 'D');
    }
    if (currentChord[KEY_z_D0][KEY_z_D1]) {
      pressKey(&firstKeyPressed, 'Z');
    }

    Keyboard.releaseAll();
  }
};

#endif // StenoKeyboardProtocol_h

