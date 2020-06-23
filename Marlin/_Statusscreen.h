/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 */
#pragma once

/**
 * Made with Marlin Bitmap Converter
 * https://marlinfw.org/tools/u8glib/converter.html
 *
 * This bitmap from the file 'CR10-V2-statusscreen.bmp'
 */
#pragma once

#define STATUS_LOGO_WIDTH 40

const unsigned char status_logo_bmp[] PROGMEM = {
  B00000000,B00000000,B00000000,B00000000,B00000000, // ........................................
  B00000000,B00000000,B00000000,B00000000,B00000000, // ........................................
  B00000000,B11110011,B11100000,B00000000,B00000000, // ........####..#####.....................
  B00000001,B11111001,B10110000,B11000011,B10000000, // .......######..##.##....##....###.......
  B00000001,B10001001,B10110001,B11000110,B11000000, // .......##...#..##.##...###...##.##......
  B00000001,B10000001,B11110000,B11000110,B11000000, // .......##......#####....##...##.##......
  B00000001,B10000001,B10111000,B11000110,B11000000, // .......##......##.###...##...##.##......
  B00000001,B11111001,B10011000,B11000110,B11000000, // .......######..##..##...##...##.##......
  B00000000,B11110011,B10011001,B11100011,B10000000, // ........####..###..##..####...###.......
  B00000000,B00000000,B00000000,B00000000,B00000000, // ........................................
  B00000000,B00000000,B00000000,B00000000,B00000000, // ........................................
  B00000000,B00000011,B10001100,B00000000,B00000000, // ..............###...##..................
  B00000000,B00000001,B10001000,B11100000,B00000000, // ...............##...#...###.............
  B00000000,B00000000,B11001001,B11110000,B00000000, // ................##..#..#####............
  B00000000,B00000000,B11011001,B00110000,B00000000, // ................##.##..#..##............
  B00000000,B00000000,B01110000,B01100000,B00000000, // .................###.....##.............
  B00000000,B00000000,B01100000,B11001000,B00000000, // .................##.....##..#...........
  B00000000,B00000000,B00100001,B11111000,B00000000, // ..................#....######...........
  B00000000,B00000000,B00000000,B00000000,B00000000  // ........................................
};


//
// Use default bitmaps
//
#define STATUS_HOTEND_ANIM
#define STATUS_BED_ANIM
#define STATUS_HEATERS_XSPACE   20
#if HOTENDS < 2
  #define STATUS_HEATERS_X      48
  #define STATUS_BED_X          72
#else
  #define STATUS_HEATERS_X      40
  #define STATUS_BED_X          80
#endif
