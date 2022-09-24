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

const Colour Colour::colours[] {
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

Colour::Colour() :
  red(0),
  green(0),
  blue(0) {}

Colour::Colour(uint8_t r, uint8_t g, uint8_t b) :
  red(r),
  green(g),
  blue(b) {}

Colour& Colour::operator+=(const Colour& rhs) {
  uint16_t r = red + rhs.red;
  if (r > 255)  r = 255;
  red = r;
  uint16_t g = green + rhs.green;
  if (g > 255)  g = 255;
  green = g;
  uint16_t b = blue + rhs.blue;
  if (b > 255)  b = 255;
  blue = b;
  return *this;
}

ColourHSV::ColourHSV() :
  hue(0),
  sat(0),
  val(0) {
  // empty
}

ColourHSV::ColourHSV(float h, float s, float v) :
  hue(h),   // hue: [0-360]
  sat(s),   // sat: [0-100]  
  val(v) {  // val: [0-100]
  if (hue < 0) hue = 0;
  if (hue > 360) hue = 360;
  if (sat < 0) sat = 0;
  if (sat > 100) sat = 100;
  if (val < 0) val = 0;
  if (val > 100) val = 100;
}

ColourHSV::operator Colour() const {
  float s = sat / 100;
  float v = val / 100;
  float c = s * v;
  float X = c * (1 - std::abs(std::fmod(hue / 60.0, 2) - 1));
  float m = v - c;
  float r;
  float g;
  float b;
  if (hue >= 0 && hue < 60) {
    r = c;
    g = X;
    b = 0;
  } else if (hue >= 60 && hue < 120) {
    r = X;
    g = c;
    b = 0;
  } else if (hue >= 120 && hue < 180) {
    r = 0;
    g = c;
    b = X;
  } else if (hue >= 180 && hue < 240) {
    r = 0;
    g = X;
    b = c;
  } else if (hue >= 240 && hue < 300) {
    r = X;
    g = 0;
    b = c;
  } else {
    r = c;
    g = 0;
    b = X;
  }
  return Colour((r + m) * 255, (g + m) * 255, (b + m) * 255);
}