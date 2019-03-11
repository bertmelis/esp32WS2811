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

#include "LarsonScanner.h"

LarsonScanner::LarsonScanner(Colour colour, uint32_t size, uint32_t speedDelay, uint32_t delay) :
  _colour(colour),
  _size(size),
  _speedDelay(speedDelay),
  _delay(delay) {}

void LarsonScanner::run(WS2811* ws2811, size_t numLeds) {
  // Move up
  size_t size = numLeds - _size - 2;
  for (size_t i = 0; i < size; ++i) {
    ws2811->setAll(0, 0, 0);
    ws2811->setPixel(i, _colour.red / 10, _colour.green / 10, _colour.blue / 10);
    for (size_t j = 1; j <= _size; ++j) {
      ws2811->setPixel(i + j, _colour.red, _colour.green, _colour.blue);
    }
    ws2811->setPixel(i + _size + 1, _colour.red / 10, _colour.green / 10, _colour.blue / 10);
    ws2811->show();
    delay(_speedDelay);
  }

  delay(_delay);

  // Move down
  for (size_t i = size; i > 0; --i) {
    ws2811->setAll(0, 0, 0);
    ws2811->setPixel(i, _colour.red / 10, _colour.green / 10, _colour.blue / 10);
    for (size_t j = 1; j <= _size; ++j) {
      ws2811->setPixel(i + j, _colour.red, _colour.green, _colour.blue);
    }
    ws2811->setPixel(i + _size + 1, _colour.red / 10, _colour.green / 10, _colour.blue / 10);
    ws2811->show();
    delay(_speedDelay);
  }

  delay(_delay);
}
