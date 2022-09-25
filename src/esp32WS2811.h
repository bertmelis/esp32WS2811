/*

Copyright 2017 Neil Kolban (Apache License 2.0)
The WS281x driver is taken from https://github.com/nkolban/esp32-snippets
Changes limited to datatypes

Copyright 2018 Bert Melis

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
 * @file esp32WS2811.h
 * @brief API definitions for WS2811 LEDs ESP32's RMT peripheral
 *
 * This library uses is made for use in the Arduino framework
 * for ESP32.
 */

#pragma once

// General
#include <stddef.h>
#include <functional>

// ESP-IDF
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <driver/rmt.h>
#include <driver/gpio.h>

// Arduino framework
#include <esp32-hal-log.h>

// Internal
#include "Effects/Colour.h"  // Colour definition
#include "Effects/Effect.h"  // includes all builtin effects

class WS2811Effect;

/**
 * @brief Create a string of ws2811 leds.
 */
class WS2811 {
 public:
  /**
   * @brief Create a string of ws2811 leds.
   * 
   * @param dataPin pin number connected to DATA line of the leds
   * @param numLeds number of leds on the string
   * @param channel RMT channel to use, defaults to channel 0
   */
  explicit WS2811(int dataPin, size_t numLeds, int channel = RMT_CHANNEL_0);

  virtual ~WS2811();

  /**
   * @brief Start the WS2811 string, turns off all the leds on this string and waits for 
   * further input.
   */
  void begin();

  /**
   * @brief Returns the number of leds.
   */
  size_t numLeds() const;

  /**
   * @brief Send the defined colours to the leds.
   * 
   * This actually formats and copies the values to the RMT driver which
   * sends them over the DATA line. By calling `show()`, the memory which holds the led's 
   * colours will be unaccessable for a very short moment to be able to copy the data to 
   * the RMT driver without errors.
   */
  void show();

  /**
   * @brief Set the colour of an individual led.
   * 
   * Call `show()` to actually send the new colour to the led. This method 
   * blocks if and untill the memory to store the colours is available for writing. 
   * 
   * @param index position on the string, zero-indexed.
   * @param colour Colour object holding new colours
   */
  void setPixel(size_t index, Colour colour);

  /**
   * @brief Set the colour of an individual led.
   * 
   * Call `show()` to actually send the new colour to the led. This method 
   * blocks if and untill the memory to store the colours is available for writing. 
   * 
   * @param index position on the string, zero-indexed.
   * @param red red value 0-255.
   * @param green green value 0-255.
   * @param blue blue value 0-255.
   */
  void setPixel(size_t index, uint8_t red, uint8_t green, uint8_t blue);

  /**
   * @brief Get the colour of an individual led.
   * 
   * Return the colour of the led at the given position.
   * Returns all zero for an out-of-bound index. Mind that this gives
   * the colour of the LED as it is in the buffer. If you did not call
   * .show(), the actual LED may be different.
   * 
   * @param index position on the string, zero-indexed.
   * @return Colour struct holding the requested pixel's colour
   */
  Colour getPixel(size_t index) const;

  /**
   * @brief Set the red colour component of an individual led.
   * 
   * Call `show()` to actually send the new colour to the led. This method 
   * blocks if and until the memory to store the colours is available for writing. 
   * 
   * @param index position on the string, zero-indexed.
   * @param red red component value 0-255
   */
  void setRed(size_t index, uint8_t red);

  /**
   * @brief Set the green colour component of an individual led.
   * 
   * Call `show()` to actually send the new colour to the led. This method 
   * blocks if and until the memory to store the colours is available for writing. 
   * 
   * @param index position on the string, zero-indexed.
   * @param green green component value 0-255
   */
  void setGreen(size_t index, uint8_t green);

  /**
   * @brief Set the blue colour component of an individual led.
   * 
   * Call `show()` to actually send the new colour to the led. This method 
   * blocks if and until the memory to store the colours is available for writing. 
   * 
   * @param index position on the string, zero-indexed.
   * @param blue blue component value 0-255
   */
  void setBlue(size_t index, uint8_t blue);

  /**
   * @brief Turn off all leds on this string.
   * 
   * Call `show()` to actually turn off the LEDs.
   */
  void clearAll();

  /**
   * @brief Set the colour of all the leds on this string.
   * 
   * Call `show()` to actually set the colour of the LEDs.
   * 
   * @param red red value 0-255.
   * @param green green value 0-255.
   * @param blue blue value 0-255.
   */
  void setAll(uint8_t red, uint8_t green, uint8_t blue);

  /**
   * @brief Set the colour of all the leds on this string.
   * 
   * Call `show()` to actually set the colour of the LEDs.
   * 
   * @param colour Colour object
   */
  void setAll(Colour colour);

  /**
   * @brief Starts an effect
   * 
   * You first have to stop a running effect before starting a new one.
   * The lib does not delete the WS2811Effect object.
   * 
   * @param effect Pointer to an effect
   */
  void startEffect(WS2811Effect* effect);

  /**
   * @brief Stops an effect
   * 
   * This methods stops a running effect. While this call return immideately, the effect only stops after
   * it's `run` functions returns.
   * The lib does not delete the stopped WS2811Effect object.
   */
  void stopEffect();

 private:
  void _setupRMT();
  static void _handleRmt(WS2811* ws2811);
  TaskHandle_t _rmtTask;
  SemaphoreHandle_t _smphr;
  rmt_channel_t _channel;
  int _dataPin;
  size_t _numLeds;
  Colour* _leds;
  WS2811Effect* _effect;
};
