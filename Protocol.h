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
 
#ifndef Protocol_h
#define Protocol_h

class Protocol {
public:

  virtual void sendChord(const boolean (&currentChord)[ROWS][COLS]) const = 0;
};

#endif // Protocol_h

