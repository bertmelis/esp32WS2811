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

#include "Autumn.h"

const ColourHSV Autumn::_colours[] = {
  {123.0, 100.0, 47.8},  // green
  { 42.0, 100.0, 47.8},  // orange
  {  1.0, 100.0, 47.8},  // red
  {295.0,  96.7, 47.8},  // purple
  { 54.0,  97.5, 47.8},  // yellow
  {178.0,  64.5, 43.1}   // blue
};
const size_t Autumn::_numberColours = 6;

Autumn::Autumn(uint32_t steps, uint32_t delay) :
  _steps(steps),
  _step(0),
  _delay(delay),
  _lastMillis(0),
  _currentColourIndex(0),
  _nextColourIndex(0) {}

void Autumn::_setup() {
  _currentColourIndex = random(_numberColours);
  _nextColourIndex = random(_numberColours);
  _ledstrip->setAll(_colours[_currentColourIndex]);
  _ledstrip->show();
  _lastMillis = millis();
}

void Autumn::_loop() {
  uint32_t currentMillis = millis();
  if (currentMillis - _lastMillis > _delay) {
    _lastMillis = currentMillis;
    _step = 0;
    _currentColourIndex = _nextColourIndex;
    _nextColourIndex = random(_numberColours);
  }
  if (_step > _steps) {
    delay(1);
    return;
  }

  ColourHSV c(_colours[_currentColourIndex].hue + (_step * 1.0 / _steps) * (_colours[_nextColourIndex].hue - _colours[_currentColourIndex].hue),
              _colours[_currentColourIndex].sat + (_step * 1.0 / _steps) * (_colours[_nextColourIndex].sat - _colours[_currentColourIndex].sat),
              _colours[_currentColourIndex].val + (_step * 1.0 / _steps) * (_colours[_nextColourIndex].val - _colours[_currentColourIndex].val));
  ++_step;
  _ledstrip->setAll(c);
  _ledstrip->show();
  delay(1);
}

void Autumn::_cleanup() {
  // nothing to clean up
}