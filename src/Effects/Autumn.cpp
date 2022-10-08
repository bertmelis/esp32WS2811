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

/*
const ColourHSV Autumn::_colours[] = {
  {123.0, 100.0, 47.8},  // green
  { 42.0, 100.0, 47.8},  // orange
  {  1.0, 100.0, 47.8},  // red
  {295.0,  96.7, 47.8},  // purple
  { 54.0,  97.5, 47.8},  // yellow
  {178.0,  64.5, 43.1}   // blue
};
*/
const Colour Autumn::_colours[] = {
  {  0, 122,   6},  // green
  {122,  85,   0},  // orange
  {122,   2,   0},  // red
  {112,   4, 122},  // purple
  {122, 110,   3},  // yellow
  { 39, 110, 108}   // blue
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
  _nextColourIndex = _currentColourIndex;
  _ledstrip->setAll(_colours[_currentColourIndex]);
  _ledstrip->show();
  _lastMillis = millis();
}

void Autumn::_loop() {
  uint32_t currentMillis = millis();
  if (currentMillis - _lastMillis > _delay) {
    _lastMillis = currentMillis;
    _startLed = random(_ledstrip->numLeds() + 1);
    _step = 0;
    _currentColourIndex = _nextColourIndex;
    _nextColourIndex = random(_numberColours);
  }

  size_t numLeds = _ledstrip->numLeds();
  if (_step > 4 * _steps) {  // TODO(bertmelis): check for max instead of arbitrary 4x number of steps
    delay(10);  // arbitrary delay to keep task idle
    return;
  }

  // assure safe unsigned to signed int conversion
  assert(numLeds <= INT_MAX);

  //int32_t maxDist = std::max(std::abs((int32_t)_startLed - 0), std::abs((int32_t)_startLed - (int32_t)numLeds));
	//int32_t stepSize = _steps / maxDist;
  int32_t stepSize = _steps / (numLeds / 2);

  for (size_t i = 0; i < numLeds; ++i) {
    // calculate distance to start
    int32_t dist = std::abs((int32_t)_startLed - (int32_t)i);
		int32_t ledStep = _step - stepSize * dist;
		if (ledStep < 0) ledStep = 0;
		if (ledStep > _steps) ledStep = _steps;
    /*
    ColourHSV c(_colours[_currentColourIndex].hue + (ledStep * 1.0 / _steps) * (_colours[_nextColourIndex].hue - _colours[_currentColourIndex].hue),
                _colours[_currentColourIndex].sat + (ledStep * 1.0 / _steps) * (_colours[_nextColourIndex].sat - _colours[_currentColourIndex].sat),
                _colours[_currentColourIndex].val + (ledStep * 1.0 / _steps) * (_colours[_nextColourIndex].val - _colours[_currentColourIndex].val));
    */
    Colour c(_colours[_currentColourIndex].red + (ledStep * 1.0 / _steps) * (_colours[_nextColourIndex].red - _colours[_currentColourIndex].red),
             _colours[_currentColourIndex].green + (ledStep * 1.0 / _steps) * (_colours[_nextColourIndex].green - _colours[_currentColourIndex].green),
             _colours[_currentColourIndex].blue + (ledStep * 1.0 / _steps) * (_colours[_nextColourIndex].blue - _colours[_currentColourIndex].blue));
    _ledstrip->setPixel(i, c);
  }
  ++_step;
  _ledstrip->show();
  delay(1);
}

void Autumn::_cleanup() {
  // nothing to clean up
}