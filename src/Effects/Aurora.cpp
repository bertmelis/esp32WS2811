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

#include "Aurora.h"

// List of colors allowed for waves
// The first dimension of this array must match the second dimension of the colorwighting array
uint8_t allowedcolors[5][3] = {
  { 17, 177, 13 },    // Greenish
  { 148, 242, 5 },    // Greenish
  { 25, 173, 121},    // Turquoise
  { 250, 77, 127 },   // Pink
  { 171, 101, 221 },  // Purple
};

// Colorweighing allows to give some colors more weight so it is more likely to be choosen for a wave.
// The second dimension of this array must match the first dimension of the allowedcolors array
// Here are 3 presets.
uint8_t colorweighting[3][5] = {
  {10, 10, 10, 10, 10},   // Weighting equal (every color is equally likely)
  {2, 2, 2, 6, 6},        // Weighting reddish (red colors are more likely)
  {6, 6, 6, 2, 2}         // Weighting greenish (green colors are more likely)
};

// Function to get the color for a wave based on the weighting.
// Paramter weighting: First index of colorweighting array. Basically what preset to choose.
uint8_t getWeightedColor(uint8_t weighting) {
  uint8_t sumOfWeights = 0;
  for (uint8_t i = 0; i < sizeof(colorweighting[0]); ++i) {
    sumOfWeights += colorweighting[weighting][i];
  }
  uint8_t randomweight = random(0, sumOfWeights);
  for(uint8_t i = 0; i < sizeof(colorweighting[0]); ++i) {
    if (randomweight < colorweighting[weighting][i]) {
      return i;
    }
    randomweight -= colorweighting[weighting][i];
  }
  return 0;  // avoid compiler warning
}

Aurora::BorealisWave::BorealisWave(size_t numLeds) :
  _numLeds(numLeds),
  _ttl(random(500, 1501)),
  _basecolor(getWeightedColor(W_COLOR_WEIGHT_PRESET)),
  _basealpha(random(50, 100) / 100.0),
  _age(0),
  _width(random(_numLeds / 10, _numLeds / W_WIDTH_FACTOR)),
  _center(random(100) / 100.0 * _numLeds),
  _goingleft(rand() > (RAND_MAX / 2)),
  _speed(random(10, 30) / 100.0 * W_SPEED_FACTOR),
  _alive(true) {}

Colour* Aurora::BorealisWave::getColorForLED(int ledIndex) {
  if(ledIndex < _center - _width / 2 || ledIndex > _center + _width / 2) {
    //Position out of range of this wave
    return nullptr;
  } else {
    Colour* rgb = new Colour;

    //Offset of this led from center of wave
    //The further away from the center, the dimmer the LED
    int offset = abs(ledIndex - _center);
    float offsetFactor = (float)offset / (_width / 2);

    //The age of the wave determines it brightness.
    //At half its maximum age it will be the brightest.
    float ageFactor = 1;        
    if((float)_age / _ttl < 0.5) {
      ageFactor = (float)_age / (_ttl / 2);
    } else {
      ageFactor = (float)(_ttl - _age) / ((float)_ttl * 0.5);
    }

    //Calculate color based on above factors and basealpha value
    rgb->red = allowedcolors[_basecolor][0] * (1 - offsetFactor) * ageFactor * _basealpha;
    rgb->green = allowedcolors[_basecolor][1] * (1 - offsetFactor) * ageFactor * _basealpha;
    rgb->blue = allowedcolors[_basecolor][2] * (1 - offsetFactor) * ageFactor * _basealpha;
  
    return rgb;
  }
}

void Aurora::BorealisWave::update() {
  if (_goingleft) {
    _center -= _speed;
  } else {
    _center += _speed;
  }
  _age++;
  if (_age > _ttl) {
    _alive = false;
  } else {
    if (_goingleft) {
      if (_center + _width / 2 < 0) {
        _alive = false;
      }
    } else {
      if (_center - _width / 2 > _numLeds) {
        _alive = false;
      }
    }
  }
}

bool Aurora::BorealisWave::stillAlive() {
  return _alive;
}

Aurora::Aurora() :
  _waves() {}

Aurora::~Aurora() {
  stop();
  _cleanup();
}

void Aurora::_setup() {
  _ledstrip->clearAll();
  _ledstrip->show();
  for (size_t i = 0; i < W_COUNT; ++i) {
    _waves[i] = new BorealisWave(_ledstrip->numLeds());
  }
}

void Aurora::_loop() {
  for (size_t i = 0; i < W_COUNT; ++i) {
    // Update values of wave
    _waves[i]->update();

    if(!(_waves[i]->stillAlive())) {
      // If a wave dies, remove it from memory and spawn a new one
      delete _waves[i];
      _waves[i] = new BorealisWave(_ledstrip->numLeds());
    }
  }

  //Loop through LEDs to determine color
  for (size_t i = 0; i < _ledstrip->numLeds(); ++i) {
    Colour mixedRgb = Colour(0,0,0);
    // For each LED we must check each wave if it is "active" at this position.
    // If there are multiple waves active on a LED we multiply their values.
    for(size_t j = 0; j < W_COUNT; ++j) {
      Colour* rgb = _waves[j]->getColorForLED(i);
      if(rgb != nullptr) {       
        mixedRgb += *rgb;
      }
      delete[] rgb;
    }
    _ledstrip->setPixel(i, mixedRgb);
  }
  _ledstrip->show();
  delay(20);
}

void Aurora::_cleanup() {
  for (size_t i = 0; i < W_COUNT; ++i) {
    delete _waves[i];
  }
} 
