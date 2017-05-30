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

#include "GeminiProtocol.h"
#include "NKROProtocol.h"
#include "TxBoltProtocol.h"

// Configuration variables
int rowPins[ROWS] = {13, 12, 11, 10, 9};
int colPins[COLS] = {8, 7, 6, 5, 4, 2};
int ledPin = 3;
long debounceMillis = 20;

// Keyboard state variables
boolean isStrokeInProgress = false;
boolean currentChord[ROWS][COLS];
boolean currentKeyReadings[ROWS][COLS];
boolean debouncingKeys[ROWS][COLS];
unsigned long debouncingMicros[ROWS][COLS];

// Other state variables
int ledIntensity = 1; // Min 0 - Max 255

// Protocol
GeminiProtocol protocolGemini;
NKROProtocol protocolNKRO;
TxBoltProtocol protocolTxBolt;
Protocol& protocol = protocolNKRO;

/**
 * Sets up the initial state.
 * This is called when the keyboard is connected.
 */
void setup() {
  Serial.begin(9600);
  for (int i = 0; i < COLS; i++) {
    pinMode(colPins[i], INPUT_PULLUP);
  }
  for (int i = 0; i < ROWS; i++) {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], HIGH);
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
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (currentKeyReadings[i][j] == true) {
        currentChord[i][j] = true;
        isAnyKeyPressed = true;
      }
    }
  }
  return isAnyKeyPressed;
}

/**
 * Checks for debouncing keys.
 * If a key is pressed, we add it to the debouncing keys and record the time.
 */
void checkNewDebouncingKeys() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (currentKeyReadings[i][j] == true && debouncingKeys[i][j] == false) {
        debouncingKeys[i][j] = true;
        debouncingMicros[i][j] = micros();
      }
    }
  }
}
/**
 * Checks already debouncing keys.
 * If a key debounces, start chord recording.
 */
void checkAlreadyDebouncingKeys() {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      if (debouncingKeys[i][j] == true && currentKeyReadings[i][j] == false) {
        debouncingKeys[i][j] = false;
        continue;
      }
      if (debouncingKeys[i][j] == true && micros() - debouncingMicros[i][j] / 1000 > debounceMillis) {
        isStrokeInProgress = true;
        currentChord[i][j] = true;
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
void clearBooleanMatrix(boolean booleanMatrix[][COLS], boolean value) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      booleanMatrix[i][j] = value;
    }
  }
}

/**
 * Reads all keys from digital I/O into a boolean matrix.
 */
void readKeys() {
  for (int i = 0; i < ROWS; i++) {
    digitalWrite(rowPins[i], LOW);
    for (int j = 0; j < COLS; j++) {
      currentKeyReadings[i][j] = digitalRead(colPins[j]) == LOW ? true : false;
    }
    digitalWrite(rowPins[i], HIGH);
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
  // If fn keys have been pressed, delegate to corresponding method and return
  if (currentChord[0][5] && currentChord[1][5]) {
    fn1fn2();
  } else if (currentChord[0][5]) {
    fn1();
  } else if (currentChord[1][5]) {
    fn2();
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
void fn1() {
  // "PH" -> Set protocol
  if (currentChord[0][2] && currentChord[0][3]) {
    // "-PB" -> NKRO Keyboard
    if (currentChord[3][1] && currentChord[4][1]) {
      protocol = protocolNKRO;
    }
    // "-G" -> Gemini PR
    else if (currentChord[4][2]) {
      protocol = protocolGemini;
    }
    // "-B" -> TX Bolt
    else if (currentChord[4][1]) {
      protocol = protocolTxBolt;
    }
  }
}

/**
 * Is called when the "fn2" key has been pressed, but not "fn1".
 *
 * Tip: maybe it is better to avoid using "fn2" key alone in order to avoid
 * accidental activation?
 *
 * Current functions: none
 */
void fn2() {

}

/**
 * Is called when both the "fn1" and the "fn2" keys have been pressed.
 * Current functions:
 *   HR-P   ->   LED intensity up
 *   HR-F   ->   LED intensity down
 */
void fn1fn2() {
  // "HR" -> Change LED intensity
  if (currentChord[0][3] && currentChord[1][3]) {
    // "-P" -> LED intensity up
    if (currentChord[3][1]) {
      ledIntensityUp();
    }
    // "-F" -> LED intensity down
    if (currentChord[3][0]) {
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

