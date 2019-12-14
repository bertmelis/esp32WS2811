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
 * @file BlinkLed.h
 * @brief BlinkLed effect definitions
 */

#pragma once

#include <stddef.h>
#include <Arduino.h>  // delay, random...
#include "Effect.h"
#include "Colour.h"

/**
 * @brief Blink first led effect.
 * 
 * Blinks first led, useful for signalling
 */
class BlinkLed : public WS2811Effect {
 public:
  /**
   * @brief Creates the effect instance
   * 
   * @param colour Colour the led will blink in.
   * @param delay time in milliseconds between fading out and fading in.
   */
  explicit BlinkLed(Colour colour, size_t pixel, uint32_t delay);

  /**
   * @brief Method which will be called by WS2811. See Effect - run.
   * 
   * @param ws2811 Pointer to the WS2811 instance (led strip) on which this effect will run.
   * @param numLeds Number of leds for the WS2811 instance.
   */
  void run(WS2811* ws2811, size_t numLeds);

 private:
  Colour _colour;
  size_t _pixel;
  uint32_t _delay;
};
