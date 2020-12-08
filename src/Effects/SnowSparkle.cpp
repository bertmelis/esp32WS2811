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

const Colour SnowSparkle::Sparkle::_flake(255, 255, 255);

SnowSparkle::Sparkle::Sparkle(SnowSparkle* parent, size_t steps):
  _parent(parent),
  _pos(random(_parent->_ledstrip->numLeds())),
  _step(1),
  _steps(steps) {}

SnowSparkle::Sparkle::~Sparkle() {
  _parent->_ledstrip->setPixel(_pos, _parent->_baseColour);
}

void SnowSparkle::Sparkle::update() {
  uint8_t red = _flake.red + ((_step / _steps) * (_parent->_baseColour.red - _flake.red));
  uint8_t green = _flake.green + ((_step / _steps) * (_parent->_baseColour.green - _flake.green));
  uint8_t blue = _flake.blue + ((_step / _steps) * (_parent->_baseColour.blue - _flake.blue));
  ++_step;
  _parent->_ledstrip->setPixel(_pos, red, green, blue);
}

bool SnowSparkle::Sparkle::finished() const {
  return (_step > _steps);
}

SnowSparkle::SnowSparkle(Colour baseColour, size_t nrSparkles, uint32_t minDelay, uint32_t maxDelay) :
  _baseColour(baseColour),
  _nrSparkles(nrSparkles),
  _sparkles(nullptr),
  _lastMillis(millis()),
  _minDelay(minDelay),
  _maxDelay(maxDelay),
  _nextDelay(random(_minDelay, _maxDelay)) {
    _sparkles = new Sparkle*[_nrSparkles];
    for (size_t i = 0; i < _nrSparkles; ++i) {
      _sparkles[i] = nullptr;
    }
  }

void SnowSparkle::_setup() {
  _ledstrip->setAll(_baseColour);
  _ledstrip->show();
  delay(10);
}

void SnowSparkle::_loop() {
  if (millis() - _lastMillis > _nextDelay) {
    _lastMillis = millis();
    _nextDelay = random(_minDelay, _maxDelay);
    for (size_t i = 0; i < _nrSparkles; ++i) {
      if (_sparkles[i] == nullptr) {
        _sparkles[i] = new Sparkle(this, random(50, 200));
      }
    }
  }
  for (size_t i = 0; i < _nrSparkles; ++i) {
    if (_sparkles[i]) {
      _sparkles[i]->update();
      if (_sparkles[i]->finished()) {
        delete _sparkles[i];
        _sparkles[i] = nullptr;
      }
    }
  }
  _ledstrip->show();
  delay(10);
}

void SnowSparkle::_cleanup() {
  for (size_t i = 0; i < _nrSparkles; ++i) {
    if (_sparkles[i]) {
      delete _sparkles[i];
      _sparkles[i] = nullptr;
    }
  }
}
