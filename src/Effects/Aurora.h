/*

Copyright (c) 2020 Matthias Folte

Adapted for this lib by Bert Melis

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


//WAVE CONFIG
#define W_COUNT 6                 // Number of simultaneous waves
#define W_SPEED_FACTOR 3          // Higher number, higher speed
#define W_WIDTH_FACTOR 1          // Higher number, smaller waves
#define W_COLOR_WEIGHT_PRESET 1   // What color weighting to choose
#define W_RANDOM_SEED 11          // Change this seed for a different pattern. If you read from
                                  // an analog input here you can get a different pattern everytime.

class Aurora : public WS2811Effect {
  class BorealisWave {
  public:
    explicit BorealisWave(size_t numLeds);
    Colour* getColorForLED(int ledIndex);
    void update();
    bool stillAlive();
    
  private:
    size_t _numLeds;
    int _ttl;
    uint8_t _basecolor;
    float _basealpha;
    int _age;
    int _width;
    float _center;
    bool _goingleft;
    float _speed;
    bool _alive;
  };

 public:
  explicit Aurora();
  ~Aurora();

 private:
  void _setup();
  void _loop();
  void _cleanup();
  BorealisWave* _waves[W_COUNT];
};
