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

#include "SnowSparkle.h"

SnowSparkle::SnowSparkle(Colour baseColour, size_t maxNoSparkles, uint32_t minDelay, uint32_t maxDelay) :
  _baseColour(baseColour),
  _maxNoSparkles(maxNoSparkles),
  _noSparkles(random(_maxNoSparkles)),
  _minDelay(minDelay),
  _maxDelay(maxDelay) {}

void SnowSparkle::_setup() {
  _ledstrip->setAll(_baseColour);
  _ledstrip->show();
  delay(20);
}

void SnowSparkle::_loop() {
  size_t numLeds = _ledstrip->numLeds();
  size_t noSparkles = random(_maxNoSparkles);
  for (size_t i = 0; i < noSparkles; ++i) {
    _ledstrip->setPixel(random(numLeds), 255, 255, 255);
  }
  _ledstrip->show();
  delay(random(_minDelay, _maxDelay));
  _setup();
}
