# esp32WS2811

Arduino Library for ESP32 to drive WS2811/WS2811 RGB leds using the RMT peripheral.

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
      |      GPIO 18 +--------------------+-->+ OE       GND  +---+--||--+----- Vdd WS2811
      |              |                    |   |     TXS0102   |   |  0.1µF
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

# Firmware

Include the header file and define your LED string:

```C++
#include <esp32WS2811.h>

// first argument is the data pin, the second argument is the number of LEDs
WS2811 yourLedString(18, 50);
```

Start the LED string:

```C++
void setup() {
  // include your other code

  yourLedString.begin();
}
```

Change LED colours:

```C++
yourLedString.setPixel(size_t index, uint32_t red, uint32_t green, uint32_t blue);
yourLedString.show();  // this updates the LEDs
```

A number of helpers methods are available:

```C++
void clearAll();  // turns off all LEDs
void setAll(uint32_t red, uint32_t green, uint32_t blue);  // gives all LEDs the specified colour
void set Bightness (uint8_t brightness, bool reset = false)  // set maximum brightness (in %), reset updates immediately
```

Keep in mind that all these methods require to call `show()` afterwards.

## Effects

Starting and stopping an effect is done by.

```C++
void startEffect(WS2811Effect* effect);
void stopEffect();
```

You don't have to stop a runnign effect before starting a new one.

Available effects are in the "Effects" directory. Creating your own is done by creating an effect class:

```C++
// this effect will light a random LED, delay and turn off
class MyNewEffect : public WS2811Effect {
 public:
  explicit FadeColours(int delay) :
    _delay(delay) {}
  void run(WS2811* strip, size_t numLeds) {
    strip->clearAll();  // clear all, we don't know the current state...
    strip->show();
    strip->setPixel(random(0, numLeds), 0, 255, 0);
    strip->show();
    delay(_delay);
  }

 private:
  int _delay;
};
```

using this effect in your code as follows:

```C++
MyNewEffect myNewEffect(500);  // make sure it doesn't go out of scope
ws2811.startEffect(&myNewEffect);
```

# Credits

The RMT driver for WS2811/WS2812 comes from @nkolban [https://github.com/nkolban/esp32-snippets](https://github.com/nkolban/esp32-snippets).
Effects are heavily inspired and copied from [https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/](https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/)
