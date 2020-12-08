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

/**
 * @file SnowSparkle.h
 * @brief Snow sparkle effect definitions
 */

#pragma once

#include <array>

#include "Effect.h"
#include "Colour.h"

class SnowSparkle : public WS2811Effect {
 private:
  class Sparkle {
   public:
    Sparkle(SnowSparkle* parent, size_t steps);
    ~Sparkle();
    void update();
    bool finished() const;

   private:
    SnowSparkle* _parent;
    size_t _pos;
    size_t _step;
    float _steps;
    const static Colour _flake;
  };

 public:
  SnowSparkle(Colour baseColour, size_t nrSparkles, uint32_t minDelay, uint32_t maxDelay);

 private:
  void _setup();
  void _loop();
  void _cleanup();

  Colour _baseColour;
  const size_t _nrSparkles;
  Sparkle** _sparkles;
  uint32_t _lastMillis;
  uint32_t _minDelay;
  uint32_t _maxDelay;
  uint32_t _nextDelay;
};
