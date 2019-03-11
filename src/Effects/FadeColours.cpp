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

#include "FadeColours.h"

FadeColours::FadeColours(uint32_t steps, uint32_t delay) :
  _steps(steps),
  _delay(delay) {}

void FadeColours::run(WS2811* ws2811, size_t numLeds) {
  Colour colour = colours[random(0, 13)];
  // Fade in
  for (uint32_t i = 0; i < _steps; ++i) {
    uint8_t red = colour.red * i / _steps;
    uint8_t green = colour.green * i / _steps;
    uint8_t blue = colour.blue * i / _steps;
    ws2811->setAll(red, green, blue);
    ws2811->show();
    delay(5);
  }
  // Fade out
  for (uint32_t i = _steps; i > 0; --i) {
    uint8_t red = colour.red * i / _steps;
    uint8_t green = colour.green * i / _steps;
    uint8_t blue = colour.blue * i / _steps;
    ws2811->setAll(red, green, blue);
    ws2811->show();
    delay(5);
  }
  delay(_delay);
}
