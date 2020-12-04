/*

Copyright 2019 Bert Melis

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONDHTTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

#include "Colour.h"

Colour::Colour() :
  red(0),
  green(0),
  blue(0) {}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b) :
  red(r),
  green(g),
  blue(b) {}

Colour colours[] {
  {0xFF, 0x00, 0x00},  /// red
  {0x80, 0x00, 0x00},  /// maroon
  {0xFF, 0xFF, 0x00},  /// yellow
  {0x80, 0x80, 0x00},  /// olive
  {0x00, 0xFF, 0x00},  /// lime
  {0x00, 0x80, 0x00},  /// green
  {0x00, 0xFF, 0xFF},  /// aqua
  {0x00, 0x80, 0x80},  /// teal
  {0x00, 0x00, 0xFF},  /// blue
  {0x00, 0x00, 0x80},  /// navy
  {0xFF, 0x00, 0xFF},  /// fuchsia
  {0x80, 0x00, 0x80}   /// purple
};
