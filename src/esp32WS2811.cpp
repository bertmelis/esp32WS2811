/*

Copyright 2017 Neil Kolban
The WS2811 driver is taken from https://github.com/nkolban/esp32-snippets

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

#include "esp32WS2811.h"

void setItem1(rmt_item32_t* item) {
  item->level0    = 1;
  item->duration0 = 10;
  item->level1    = 0;
  item->duration1 = 6;
}

void setItem0(rmt_item32_t* item) {
  item->level0    = 1;
  item->duration0 = 4;
  item->level1    = 0;
  item->duration1 = 8;
}

void setTerminator(rmt_item32_t* item) {
  item->level0    = 0;
  item->duration0 = 50;
  item->level1    = 0;
  item->duration1 = 0;
}

WS2811::WS2811(int dataPin, size_t numLeds, int channel) :
  _rmtTask(nullptr),
  _smphr(nullptr),
  _channel(static_cast<rmt_channel_t>(channel)),
  _dataPin(dataPin),
  _numLeds(numLeds),
  _leds(nullptr),
  _effect(nullptr) {
    _leds = new Colour[_numLeds];
  }

WS2811::~WS2811() {
  stopEffect();
  vTaskDelete(_rmtTask);
  vSemaphoreDelete(_smphr);
  delete[] _leds;
}

void WS2811::begin() {
  _setupRMT();
  _smphr = xSemaphoreCreateBinary();
  xSemaphoreGive(_smphr);  // release emaphores for first use
  xTaskCreate((TaskFunction_t)&_handleRmt, "rmtTask", 2000, this, 1, &_rmtTask);
}

size_t WS2811::numLeds() const {
  return _numLeds;
}

void WS2811::show() {
  xTaskNotifyGive(_rmtTask);
}

void WS2811::setPixel(size_t index, Colour colour) {
  if (xSemaphoreTake(_smphr, 100) == pdTRUE) {
    if (index < _numLeds) {
      _leds[index] = colour;
    } else {
      log_w("setting pixel outside range");
    }
    xSemaphoreGive(_smphr);
  } else {
    log_e("could not set pixel");
  }
}

void WS2811::setPixel(size_t index, uint8_t red, uint8_t green, uint8_t blue) {
  Colour c(red, green, blue);
  setPixel(index, c);
}

Colour WS2811::getPixel(size_t index) const {
  if (index < _numLeds) {
    return _leds[index];
  }
  Colour c;
  return c;
}

void WS2811::setRed(size_t index, uint8_t red) {
  if (xSemaphoreTake(_smphr, 100) == pdTRUE) {
    if (index < _numLeds) {
      _leds[index].red = red;
    } else {
      log_w("setting pixel outside range");
    }
    xSemaphoreGive(_smphr);
  } else {
    log_e("could not set pixel");
  }
}

void WS2811::setGreen(size_t index, uint8_t green) {
  if (xSemaphoreTake(_smphr, 100) == pdTRUE) {
    if (index < _numLeds) {
      _leds[index].green = green;
    } else {
      log_w("setting pixel outside range");
    }
    xSemaphoreGive(_smphr);
  } else {
    log_e("could not set pixel");
  }
}

void WS2811::setBlue(size_t index, uint8_t blue) {
  if (xSemaphoreTake(_smphr, 100) == pdTRUE) {
    if (index < _numLeds) {
      _leds[index].blue = blue;
    } else {
      log_w("setting pixel outside range");
    }
    xSemaphoreGive(_smphr);
  } else {
    log_e("could not set pixel");
  }
}

void WS2811::clearAll() {
  Colour c;  // initializes to rgb(0,0,0)
  setAll(c);
}

void WS2811::setAll(uint8_t red, uint8_t green, uint8_t blue) {
  for (size_t i = 0; i < _numLeds; ++i) {
    setPixel(i, red, green, blue);
  }
}

void WS2811::setAll(Colour colour) {
  for (size_t i = 0; i < _numLeds; ++i) {
    setPixel(i, colour);
  }
}

void WS2811::startEffect(WS2811Effect* effect) {
  if (!effect) {
    log_w("Empty effect ptr: effect not started");
    return;  // avoids check for nullptr on each loop
  }
  if (_effect) {
    stopEffect();
  }
  _effect = effect;
  _effect->start(this);
}

void WS2811::stopEffect() {
  if (!_effect) {
    log_w("No effect available: unable to stop");
    return;
  }
  _effect->stop();
  _effect = nullptr;
}

void WS2811::_setupRMT() {
  static rmt_config_t config;
  config.rmt_mode                  = RMT_MODE_TX;
  config.channel                   = _channel;
  config.gpio_num                  = static_cast<gpio_num_t>(_dataPin);
  config.mem_block_num             = 8 - _channel;
  config.clk_div                   = 8;
  config.tx_config.loop_en         = 0;
  config.tx_config.carrier_en      = 0;
  config.tx_config.idle_output_en  = 1;
  config.tx_config.idle_level      = (rmt_idle_level_t)0;
  config.tx_config.carrier_freq_hz = 10000;
  config.tx_config.carrier_level   = (rmt_carrier_level_t)1;
  config.tx_config.carrier_duty_percent = 50;
  rmt_config(&config);
  rmt_driver_install(_channel, 0, 0);
}

void WS2811::_handleRmt(WS2811* ws2811) {
  static rmt_item32_t* rmtItems = new rmt_item32_t[ws2811->_numLeds * 24 + 1];
  static rmt_item32_t* currentItem = &rmtItems[0];

  while (true) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // clears all flags, blocks on next call
    currentItem = &rmtItems[0];
    if (xSemaphoreTake(ws2811->_smphr, 100) == pdTRUE) {
      for (size_t i = 0; i < ws2811->_numLeds; ++i) {
        uint32_t currentPixel = ws2811->_leds[i].green << 16 |
                                ws2811->_leds[i].red << 8 |
                                ws2811->_leds[i].blue;
        for (int8_t j = 23; j >= 0; --j) {
          // We have 24 bits of data representing the red, green and blue channels. The value of the
          // 24 bits to output is in the variable current_pixel.  We now need to stream this value
          // through RMT in most significant bit first.  To do this, we iterate through each of the 24
          // bits from MSB to LSB.
          if (currentPixel & (1 << j)) {
            setItem1(currentItem);
          } else {
            setItem0(currentItem);
          }
          ++currentItem;
        }
      }
      setTerminator(currentItem);  // Write the RMT terminator.
      ESP_ERROR_CHECK(rmt_write_items(ws2811->_channel, rmtItems, ws2811->_numLeds * 24, 1 /* wait till done */));
      xSemaphoreGive(ws2811->_smphr);
    } else {
      log_e("could not write RMT data");
    }
  }
}

/*
void WS2811::_handleEffect(WS2811* ws2811) {
  while (true) {
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);  // clears all flags, blocks on next call
    ws2811->_runEffect = true;
    ws2811->_effect->setup(ws2811, ws2811->_numLeds);
    while (true) {
      ws2811->_effect->run(ws2811, ws2811->_numLeds);
      if (xSemaphoreTake(ws2811->_effectSmphr, 0) == pdTRUE) {
        xSemaphoreGive(ws2811->_effectSmphr);  // prevent stopping on next effect
        ws2811->_runEffect = false;
        break;
      }
    }
  }
}
*/
