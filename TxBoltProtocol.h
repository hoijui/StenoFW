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

  virtual void sendChord(const boolean (&currentChord)[ROWS][COLS]) const {
    byte chordBytes[] = {B0, B0, B0, B0, B0};
    int index = 0;
  
    // byte 1
    // S-
    if (currentChord[KEY_S1_D0][KEY_S1_D1] || currentChord[KEY_S2_D0][KEY_S2_D1]) {
      chordBytes[index] |= B00000001;
    }
    // T-
    if (currentChord[KEY_T_D0][KEY_T_D1]) {
      chordBytes[index] |= B00000010;
    }
    // K-
    if (currentChord[KEY_K_D0][KEY_K_D1]) {
      chordBytes[index] |= B00000100;
    }
    // P-
    if (currentChord[KEY_P_D0][KEY_P_D1]) {
      chordBytes[index] |= B00001000;
    }
    // W-
    if (currentChord[KEY_W_D0][KEY_W_D1]) {
      chordBytes[index] |= B00010000;
    }
    // H-
    if (currentChord[KEY_H_D0][KEY_H_D1]) {
      chordBytes[index] |= B00100000;
    }
    // Increment the index if the current byte has any keys set.
    if (chordBytes[index]) {
      index++;
    }
  
    // byte 2
    // R-
    if (currentChord[KEY_H_D0][KEY_H_D1]) {
      chordBytes[index] |= B01000001;
    }
    // A
    if (currentChord[KEY_a_D0][KEY_a_D1]) {
      chordBytes[index] |= B01000010;
    }
    // O
    if (currentChord[KEY_o_D0][KEY_o_D1]) {
      chordBytes[index] |= B01000100;
    }
    // *
    if (currentChord[KEY_STAR1_D0][KEY_STAR1_D1] || currentChord[KEY_STAR2_D0][KEY_STAR2_D1]) {
      chordBytes[index] |= B01001000;
    }
    // E
    if (currentChord[KEY_e_D0][KEY_e_D1]) {
      chordBytes[index] |= B01010000;
    }
    // U
    if (currentChord[KEY_e_D0][KEY_e_D1]) {
      chordBytes[index] |= B01100000;
    }
    // Increment the index if the current byte has any keys set.
    if (chordBytes[index]) {
      index++;
    }
  
    // byte 3
    // -F
    if (currentChord[KEY_f_D0][KEY_f_D1]) {
      chordBytes[index] |= B10000001;
    }
    // -R
    if (currentChord[KEY_r_D0][KEY_r_D1]) {
      chordBytes[index] |= B10000010;
    }
    // -P
    if (currentChord[KEY_p_D0][KEY_p_D1]) {
      chordBytes[index] |= B10000100;
    }
    // -B
    if (currentChord[KEY_b_D0][KEY_b_D1]) {
      chordBytes[index] |= B10001000;
    }
    // -L
    if (currentChord[KEY_l_D0][KEY_l_D1]) {
      chordBytes[index] |= B10010000;
    }
    // -G
    if (currentChord[KEY_g_D0][KEY_g_D1]) {
      chordBytes[index] |= B10100000;
    }
    // Increment the index if the current byte has any keys set.
    if (chordBytes[index]) {
      index++;
    }
  
    // byte 4
    // -T
    if (currentChord[KEY_t_D0][KEY_t_D1]) {
      chordBytes[index] |= B11000001;
    }
    // -S
    if (currentChord[KEY_s_D0][KEY_s_D1]) {
      chordBytes[index] |= B11000010;
    }
    // -D
    if (currentChord[KEY_d_D0][KEY_d_D1]) {
      chordBytes[index] |= B11000100;
    }
    // -Z
    if (currentChord[KEY_z_D0][KEY_z_D1]) {
      chordBytes[index] |= B11001000;
    }
    // #
    if (currentChord[KEY_SHARP_D0][KEY_SHARP_D1]) {
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

