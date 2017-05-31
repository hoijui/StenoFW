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
 * Copyright 2014 Emanuele Caruso. See the LICENSE file for details.
 */

#define PROTOCOL_SUPPORT_GEMINI
#define PROTOCOL_SUPPORT_NKRO
#define PROTOCOL_SUPPORT_TX_BOLT

//#define PROTOCOL_DEFAULT protocolGemini
#define PROTOCOL_DEFAULT protocolNKRO
//#define PROTOCOL_DEFAULT protocolTxBolt

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

#ifdef PROTOCOL_SUPPORT_GEMINI
  #include "GeminiProtocol.h"
#endif
#ifdef PROTOCOL_SUPPORT_NKRO
  #include "NKROProtocol.h"
#endif
#ifdef PROTOCOL_SUPPORT_TX_BOLT
  #include "TxBoltProtocol.h"
#endif

// Configuration variables
const int rowPins[ROWS] = {13, 12, 11, 10, 9};
const int colPins[COLS] = {8, 7, 6, 5, 4, 2};
const int ledPin = 3;
const long debounceMillis = 20;

// Keyboard state variables
boolean isStrokeInProgress = false;
boolean currentChord[ROWS][COLS];
boolean currentKeyReadings[ROWS][COLS];
boolean debouncingKeys[ROWS][COLS];
unsigned long debouncingMicros[ROWS][COLS];

// Other state variables
int ledIntensity = 1; // Min 0 - Max 255

// Protocols
#ifdef PROTOCOL_SUPPORT_GEMINI
GeminiProtocol protocolGemini;
#endif
#ifdef PROTOCOL_SUPPORT_NKRO
NKROProtocol protocolNKRO;
#endif
#ifdef PROTOCOL_SUPPORT_TX_BOLT
TxBoltProtocol protocolTxBolt;
#endif
Protocol& protocol = PROTOCOL_DEFAULT;

/**
 * Sets up the initial state.
 * This is called when the keyboard is connected.
 */
void setup() {
#if defined(PROTOCOL_SUPPORT_GEMINI) || defined(PROTOCOL_SUPPORT_TX_BOLT)
  Serial.begin(9600);
#endif
  for (int column = 0; column < COLS; column++) {
    pinMode(colPins[column], INPUT_PULLUP);
  }
  for (int row = 0; row < ROWS; row++) {
    pinMode(rowPins[row], OUTPUT);
    digitalWrite(rowPins[row], HIGH);
  }
  pinMode(ledPin, OUTPUT);
  analogWrite(ledPin, ledIntensity);
  clearBooleanMatrixes();
}

/**
 * Reads key states and handles all chord events.
 * This run in an endless loop.
 */
void loop() {
  readKeys();

  boolean isAnyKeyPressed = true;

  // If stroke is not in progress, check debouncing keys
  if (!isStrokeInProgress) {
    checkAlreadyDebouncingKeys();
    if (!isStrokeInProgress) checkNewDebouncingKeys();
  }

  // If any key was pressed, record all pressed keys
  if (isStrokeInProgress) {
    isAnyKeyPressed = recordCurrentKeys();
  }

  // If all keys have been released, send the chord and reset global state
  if (!isAnyKeyPressed) {
    sendChord();
    clearBooleanMatrixes();
    isStrokeInProgress = false;
  }
}

/**
 * Records all pressed keys into the current chord.
 * @return false if no key is currently pressed
 */
boolean recordCurrentKeys() {
  boolean isAnyKeyPressed = false;
  for (int row = 0; row < ROWS; row++) {
    for (int column = 0; column < COLS; column++) {
      if (currentKeyReadings[row][column] == true) {
        currentChord[row][column] = true;
        isAnyKeyPressed = true;
      }
    }
  }
  return isAnyKeyPressed;
}

/**
 * Checks for debouncing keys.
 * If a key is pressed, we add it to the debouncing keys and record the time.
 * @see https://en.wikipedia.org/wiki/Keyboard_technology#Debouncing
 */
void checkNewDebouncingKeys() {
  for (int row = 0; row < ROWS; row++) {
    for (int column = 0; column < COLS; column++) {
      if (currentKeyReadings[row][column] == true && debouncingKeys[row][column] == false) {
        debouncingKeys[row][column] = true;
        debouncingMicros[row][column] = micros();
      }
    }
  }
}
/**
 * Checks already debouncing keys.
 * If a key debounces, start chord recording.
 */
void checkAlreadyDebouncingKeys() {
  for (int row = 0; row < ROWS; row++) {
    for (int column = 0; column < COLS; column++) {
      if (debouncingKeys[row][column] == true && currentKeyReadings[row][column] == false) {
        debouncingKeys[row][column] = false;
        continue;
      }
      if (debouncingKeys[row][column] == true && micros() - debouncingMicros[row][column] / 1000 > debounceMillis) {
        isStrokeInProgress = true;
        currentChord[row][column] = true;
        return;
      }
    }
  }
}

/**
 * Sets all values of all boolean matrixes to false.
 */
void clearBooleanMatrixes() {
  clearBooleanMatrix(currentChord, false);
  clearBooleanMatrix(currentKeyReadings, false);
  clearBooleanMatrix(debouncingKeys, false);
}

/**
 * Sets all values of the passed matrix to the given value.
 */
void clearBooleanMatrix(boolean booleanMatrix[][COLS], const boolean value) {
  for (int row = 0; row < ROWS; row++) {
    for (int column = 0; column < COLS; column++) {
      booleanMatrix[row][column] = value;
    }
  }
}

/**
 * Reads all keys from digital I/O into a boolean matrix.
 */
void readKeys() {
  for (int row = 0; row < ROWS; row++) {
    digitalWrite(rowPins[row], LOW);
    for (int column = 0; column < COLS; column++) {
      currentKeyReadings[row][column] = digitalRead(colPins[column]) == LOW ? true : false;
    }
    digitalWrite(rowPins[row], HIGH);
  }
}

/**
 * Sends the chord using the current protocol.
 * If there are fn keys pressed, delegate to the corresponding function instead.
 * In future versions, there should also be a way to handle fn key presses
 * before they are released,
 * eg. for mouse emulation functionality or custom key presses.
 */
void sendChord() {
  // If fn keys have been pressed, delegate to the corresponding method and return
  if (currentChord[KEY_FN1_D0][KEY_FN1_D1] && currentChord[KEY_FN2_D0][KEY_FN2_D1]) {
    pressedFn1Fn2();
  } else if (currentChord[KEY_FN1_D0][KEY_FN1_D1]) {
    pressedFn1();
  } else if (currentChord[KEY_FN2_D0][KEY_FN2_D1]) {
    pressedFn2();
  } else {
    protocol.sendChord(currentChord);
  }
}

/**
 * Is called when the "fn1" key has been pressed, but not "fn2".
 * Tip: maybe it is better to avoid using "fn1" key alone in order to avoid
 * accidental activation?
 *
 * Current functions:
 *   PH-PB  ->   Set NKRO Keyboard emulation mode
 *   PH-G   ->   Set Gemini PR protocol mode
 *   PH-B   ->   Set TX Bolt protocol mode
 */
void pressedFn1() {
#if defined(PROTOCOL_SUPPORT_GEMINI) || defined(PROTOCOL_SUPPORT_NKRO) || defined(PROTOCOL_SUPPORT_TX_BOLT)
  // "PH" -> Set protocol
  if (currentChord[KEY_P_D0][KEY_P_D1] && currentChord[KEY_H_D0][KEY_H_D1]) {
  #ifdef PROTOCOL_SUPPORT_GEMINI
    if (currentChord[KEY_g_D0][KEY_g_D1]) {
      protocol = protocolGemini;
    }
  #endif
  #ifdef PROTOCOL_SUPPORT_NKRO
    // "-PB" -> NKRO Keyboard
    if (currentChord[KEY_p_D0][KEY_p_D1] && currentChord[KEY_b_D0][KEY_b_D1]) {
      protocol = protocolNKRO;
    }
  #endif
  #ifdef PROTOCOL_SUPPORT_TX_BOLT
    // "-B" -> TX Bolt
    if (currentChord[KEY_b_D0][KEY_b_D1]) {
      protocol = protocolTxBolt;
    }
  #endif
  }
#endif
}

/**
 * Is called when the "fn2" key has been pressed, but not "fn1".
 *
 * Tip: maybe it is better to avoid using "fn2" key alone in order to avoid
 * accidental activation?
 *
 * Current functions: none
 */
void pressedFn2() {

}

/**
 * Is called when both the "fn1" and the "fn2" keys have been pressed.
 * Current functions:
 *   HR-P   ->   LED intensity up
 *   HR-F   ->   LED intensity down
 */
void pressedFn1Fn2() {
  // "HR" -> Change LED intensity
  if (currentChord[KEY_H_D0][KEY_H_D1] && currentChord[KEY_R_D0][KEY_R_D1]) {
    // "-P" -> LED intensity up
    if (currentChord[KEY_p_D0][KEY_p_D1]) {
      ledIntensityUp();
    }
    // "-F" -> LED intensity down
    if (currentChord[KEY_f_D0][KEY_f_D1]) {
      ledIntensityDown();
    }
  }
}

/**
 * Increases the LED intensity.
 * This includes roll-over, meaning it goes from full brightness back to off.
 */
void ledIntensityUp() {
  if (ledIntensity == 0) {
    ledIntensity += 1;
  } else if (ledIntensity < 50) {
    ledIntensity += 10;
  } else {
    ledIntensity += 30;
  }
  if (ledIntensity > 255) {
    ledIntensity = 0;
  }
  analogWrite(ledPin, ledIntensity);
}

/**
 * Decreases the LED intensity.
 * This includes roll-over, meaning it goes from off back to full brightness.
 */
void ledIntensityDown() {
  if (ledIntensity == 0) {
    ledIntensity = 255;
  } else if (ledIntensity < 50) {
    ledIntensity -= 10;
  } else {
    ledIntensity -= 30;
  }
  if (ledIntensity < 1) {
    ledIntensity = 0;
  }
  analogWrite(ledPin, ledIntensity);
}

