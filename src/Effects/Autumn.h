/*

Copyright 2022 Bert Melis

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

#pragma once

#include "Effect.h"
#include "Colour.h"

class Autumn : public WS2811Effect {
 public:
  explicit Autumn(uint32_t steps, uint32_t delay);

 private:
  void _setup();
  void _loop();
  void _cleanup();

 private:
  // static const ColourHSV _colours[];
  static const Colour _colours[];
  static const size_t _numberColours;
  size_t _startLed;
  uint32_t _steps;
  uint32_t _step;
  uint32_t _delay;
  uint32_t _lastMillis;
  uint8_t _currentColourIndex;
  uint8_t _nextColourIndex;
};
