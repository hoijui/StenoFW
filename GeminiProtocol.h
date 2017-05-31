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
    if (currentChord[KEY_S1_D0][KEY_S1_D1] || currentChord[KEY_S2_D0][KEY_S2_D1]) {
      chordBytes[1] += B01000000;
    }
    if (currentChord[KEY_T_D0][KEY_T_D1]) {
      chordBytes[1] += B00010000;
    }
    if (currentChord[KEY_K_D0][KEY_K_D1]) {
      chordBytes[1] += B00001000;
    }
    if (currentChord[KEY_P_D0][KEY_P_D1]) {
      chordBytes[1] += B00000100;
    }
    if (currentChord[KEY_W_D0][KEY_W_D1]) {
      chordBytes[1] += B00000010;
    }
    if (currentChord[KEY_H_D0][KEY_H_D1]) {
      chordBytes[1] += B00000001;
    }
  
    // Byte 2
    if (currentChord[KEY_R_D0][KEY_R_D1]) {
      chordBytes[2] += B01000000;
    }
    if (currentChord[KEY_a_D0][KEY_a_D1]) {
      chordBytes[2] += B00100000;
    }
    if (currentChord[KEY_o_D0][KEY_o_D1]) {
      chordBytes[2] += B00010000;
    }
    if (currentChord[KEY_STAR1_D0][KEY_STAR1_D1] || currentChord[KEY_STAR2_D0][KEY_STAR2_D1]) {
      chordBytes[2] += B00001000;
    }
  
    // Byte 3
    if (currentChord[KEY_e_D0][KEY_e_D1]) {
      chordBytes[3] += B00001000;
    }
    if (currentChord[KEY_u_D0][KEY_u_D1]) {
      chordBytes[3] += B00000100;
    }
    if (currentChord[KEY_f_D0][KEY_f_D1]) {
      chordBytes[3] += B00000010;
    }
    if (currentChord[KEY_r_D0][KEY_r_D1]) {
      chordBytes[3] += B00000001;
    }
  
    // Byte 4
    if (currentChord[KEY_p_D0][KEY_p_D1]) {
      chordBytes[4] += B01000000;
    }
    if (currentChord[KEY_b_D0][KEY_b_D1]) {
      chordBytes[4] += B00100000;
    }
    if (currentChord[KEY_l_D0][KEY_l_D1]) {
      chordBytes[4] += B00010000;
    }
    if (currentChord[KEY_g_D0][KEY_g_D1]) {
      chordBytes[4] += B00001000;
    }
    if (currentChord[KEY_t_D0][KEY_t_D1]) {
      chordBytes[4] += B00000100;
    }
    if (currentChord[KEY_s_D0][KEY_s_D1]) {
      chordBytes[4] += B00000010;
    }
    if (currentChord[KEY_d_D0][KEY_d_D1]) {
      chordBytes[4] += B00000001;
    }
  
    // Byte 5
    if (currentChord[KEY_z_D0][KEY_z_D1]) {
      chordBytes[5] += B00000001;
    }
  
    // Send chord bytes over serial
    for (int i = 0; i < 6; i++) {
      Serial.write(chordBytes[i]);
    }
  }
};

#endif // GeminiProtocol_h

