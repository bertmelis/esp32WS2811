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
 * @file Colour.h
 * @brief Colour class definitions, basic colour definitions and a ColourHSV class whit implicit conversion to RGB-Colour
 */

#pragma once

#include <stdint.h>
#include <cmath>  // abs, fmod

/**
 * @brief Class to hold RGB values. 
 */
class Colour {
 public:
  /**
   * @brief Create a colour.
   * 
   * When no arguments are given to instantiate the class, the colour 
   * will be set to black (zero value for red, green and blue)
   */
  Colour();

  /**
   * @brief Create a colour with the given values.
   * 
   * @param r red value, 0-255
   * @param g green value, 0-255
   * @param b blue value, 0-255
   */
  Colour(uint8_t r, uint8_t g, uint8_t b);

  uint8_t red;    ///< red value, 0-255
  uint8_t green;  ///< green value, 0-255
  uint8_t blue;   ///< blue value, 0-255

  /**
   * @brief Adds a colour to this colour.
   * 
   * @param rhs colour to add
   */
  Colour& operator+=(const Colour& rhs);

  /**
   * @brief Array that holds basic colours. 
   */
  static const Colour colours[12];
};

/**
 * @brief Class to hold HSV values with conversion to RGB
 */
class ColourHSV {
 public:
  /**
   * @brief Create a colour.
   * 
   * When no arguments are given to instantiate the class, the colour 
   * will be set to black (zero value for hue, saturationn and value)
   */
  ColourHSV();

  /**
   * @brief Create a colour with the given values.
   * 
   * @param h hue, 0-360
   * @param s saturation, 0-100
   * @param v value, 0-100
   */
  ColourHSV(float h, float s, float v);

  /**
   * @brief Implicit conversion from ColourHSV to RGB Colour 
   */
  operator Colour() const;

 float hue;   ///< hue, 0-360
 float sat;   ///< saturation, 0-1 (converted from percentage to absolute value)
 float val;   ///< value, 0-1 (converted from percentage to absolute value)
};
