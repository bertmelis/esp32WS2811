#include "Effect.h"

WS2811Effect::WS2811Effect() :
  _task(nullptr),
  _ledstrip(nullptr) {}

WS2811Effect::~WS2811Effect() {
  stop();
}

void WS2811Effect::start(WS2811* ledstrip) {
  _ledstrip = ledstrip;
  xTaskCreate((TaskFunction_t)&_effectTask, "effectTask", 2048, this, 1, &_task);
}

void WS2811Effect::stop() {
  if (!_task) return;
  vTaskDelete(_task);
  _cleanup();
}

void WS2811Effect::_effectTask(WS2811Effect* e) {
  e->_setup();
  while(1) {
    e->_loop();
  }
  vTaskDelete(nullptr);
}
