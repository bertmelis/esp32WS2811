# esp32WS2811

Arduino Library for ESP32 to drive WS2811/WS2812 RGB leds using the RMT peripheral. The lib is lean and only drives the leds.

## Hardware

As the ESP32 uses 3.3V levels and the WS2811 uses 5V, we need a level shifting device.
I'm successfully using a TXS-0102 voltage-level translator. I've tested up to 50 leds and 
wire lengths to the first LED up to 2 meters.

Example circuit:

```
                                  3.3V                                  5V
                                    |                                    |
                                    |                                    |
      +--------------+              |         +---------------+          |
      |          Vin +--------------+---------+ Vcca     Vccb +----------+
      |              |                        |               |          |
      |      GPIO 23 +--------------------+-->+ OE       GND  +---+--||--+----- Vdd WS2811
      |     ESP32    |                    |   |     TXS0102   |   |  0.1µF
      |      GPIO 18 +----------------------->+ A1         B1 +---------------- DATA WS2811
      |              |                    |   |               |   |
      |     GND      |                    |   | A2         B2 |   +------------ GND WS2811
      +------+-------+                    |   +---------------+   |
             |                            |                       |
             |                            +--/\/\/\---+-------- --+
             |                                10k     |
             +----------------------------------------+
                                                      |
                                                     ---
```

The pulldown resistor on OE (output enable) prevents the output
to float during startup.
Unused pins are left unconnected as they have internal 10k pullups.

## Firmware

Include the header file and define your LED string:

```cpp
#include <esp32WS2811.h>

// first argument is the data pin, the second argument is the number of LEDs
WS2811 yourLedString(18, 50);
```

Start the LED string:

```cpp
void setup() {
  // include your other code

  yourLedString.begin();
}
```

Change LED colours:

```cpp
Colour colour = yourLedString.getPixel(size_t index);  // gives you the Colour of the led on index.
yourLedString.setPixel(size_t index, uint32_t red, uint32_t green, uint32_t blue);
yourLedString.show();  // this actually makes the LEDs light up
```

A number of helpers methods are available:

```cpp
yourLedString.clearAll();  // turns off all LEDs
yourLedString.setAll(Colour colour);  // // gives all LEDs the specified colour
yourLedString.setAll(uint32_t red, uint32_t green, uint32_t blue);  // gives all LEDs the specified colour
```

Keep in mind that all these methods require to call `show()` afterwards.

## Effects

Starting and stopping an effect is done by:

```cpp
void startEffect(WS2811Effect* effect);
void stopEffect();
```

You don't have to stop a running effect before starting a new one. The effect stops immediately and does not wait for it's routine to complete.

## Sample application

You can find a full working application in this repo: [ledController](https://github.com/bertmelis/ledController)

## Credits

- The RMT driver for WS2811/WS2812 comes from @nkolban [https://github.com/nkolban/esp32-snippets](https://github.com/nkolban/esp32-snippets).
- Aurora effect source from https://github.com/Mazn1191/Arduino-Borealis
