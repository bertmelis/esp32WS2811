#include <vector>

#include <Arduino.h>
#include <Ticker.h>

#include <esp32WS2811.h>

BlinkLed signal({0, 127, 0}, 1, 500);

WS2811 ws2811(18, 50);
std::vector<WS2811Effect*> effects;
Ticker timer;

void nextEffect() {
  Serial.print("starting next effect\n");
  ws2811.startEffect(effects[random(0, effects.size())]);
}

void setup() {
  delay(5000);
  Serial.begin(115200);
  Serial.println("Booting");

  // add effects
  effects.push_back(new TwinkleRandom(10, 1000));
  effects.push_back(new RandomColours(1000));
  effects.push_back(new RandomColours(100));
  effects.push_back(new FadeColours(100, 1000));
  effects.push_back(new LarsonScanner({255, 0, 0}, 4, 10, 5));
  effects.push_back(new SnowSparkle({20, 17, 10}, 3, 100, 500));

  // output enable level shifter
  pinMode(23, OUTPUT);
  digitalWrite(23, HIGH);

  ws2811.begin();
  ws2811.startEffect(&signal);

  timer.attach(15, nextEffect);
}

void loop() {
  // nothing to do here
  delay(1);
}
