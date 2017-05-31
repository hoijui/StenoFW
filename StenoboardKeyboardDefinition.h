/*
 * StenoFW is a firmware for Stenoboard keyboards.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright 2017 Emanuele Caruso. See the LICENSE file for details.
 */

/*
 * This file defines the Stenobard hardware (stenoboard.com)
 * for the StenoFW firmware.
 */

#ifndef StenoboardKeyboardDefinition_h
#define StenoboardKeyboardDefinition_h

/** Number of key rows of our keyboard hardware */
extern const int ROWS = 5;
/** Number of key columns of our keyboard hardware */
extern const int COLS = 6;

/* The following matrix is shown here for reference only.
char keys[ROWS][COLS] = {
  {'S', 'T', 'P', 'H', '*', Fn1},
  {'S', 'K', 'W', 'R', '*', Fn2},
  {'a', 'o', 'e', 'u', '#'},
  {'f', 'p', 'l', 't', 'd'},
  {'r', 'b', 'g', 's', 'z'}
};*/

// row 0
extern const int KEY_S1_D0 = 0;
extern const int KEY_S1_D1 = 0;
extern const int KEY_T_D0 = 0;
extern const int KEY_T_D1 = 1;
extern const int KEY_P_D0 = 0;
extern const int KEY_P_D1 = 2;
extern const int KEY_H_D0 = 0;
extern const int KEY_H_D1 = 3;
extern const int KEY_STAR1_D0 = 0;
extern const int KEY_STAR1_D1 = 4;
extern const int KEY_FN1_D0 = 0;
extern const int KEY_FN1_D1 = 5;

// row 1
extern const int KEY_S2_D0 = 1;
extern const int KEY_S2_D1 = 0;
extern const int KEY_K_D0 = 1;
extern const int KEY_K_D1 = 1;
extern const int KEY_W_D0 = 1;
extern const int KEY_W_D1 = 2;
extern const int KEY_R_D0 = 1;
extern const int KEY_R_D1 = 3;
extern const int KEY_STAR2_D0 = 1;
extern const int KEY_STAR2_D1 = 4;
extern const int KEY_FN2_D0 = 1;
extern const int KEY_FN2_D1 = 5;

// row 2
extern const int KEY_a_D0 = 2;
extern const int KEY_a_D1 = 0;
extern const int KEY_o_D0 = 2;
extern const int KEY_o_D1 = 1;
extern const int KEY_e_D0 = 2;
extern const int KEY_e_D1 = 2;
extern const int KEY_u_D0 = 2;
extern const int KEY_u_D1 = 3;
extern const int KEY_SHARP_D0 = 2;
extern const int KEY_SHARP_D1 = 4;

// row 3
extern const int KEY_f_D0 = 3;
extern const int KEY_f_D1 = 0;
extern const int KEY_p_D0 = 3;
extern const int KEY_p_D1 = 1;
extern const int KEY_l_D0 = 3;
extern const int KEY_l_D1 = 2;
extern const int KEY_t_D0 = 3;
extern const int KEY_t_D1 = 3;
extern const int KEY_d_D0 = 3;
extern const int KEY_d_D1 = 4;

// row 4
extern const int KEY_r_D0 = 4;
extern const int KEY_r_D1 = 0;
extern const int KEY_b_D0 = 4;
extern const int KEY_b_D1 = 1;
extern const int KEY_g_D0 = 4;
extern const int KEY_g_D1 = 2;
extern const int KEY_s_D0 = 4;
extern const int KEY_s_D1 = 3;
extern const int KEY_z_D0 = 4;
extern const int KEY_z_D1 = 4;

const int rowPins[ROWS] = {13, 12, 11, 10, 9};
const int colPins[COLS] = {8, 7, 6, 5, 4, 2};
const int ledPin = 3;
const long debounceMillis = 20;

#endif // StenoboardKeyboardDefinition_h

