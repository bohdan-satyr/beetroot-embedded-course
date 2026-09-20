#include "Led.h"

Led::Led(uint8_t pin) : _pin(pin) {}

void Led::init() const {
    pinMode(_pin, OUTPUT);
}

void Led::set(LedState state) const {
    digitalWrite(_pin, static_cast<uint8_t>(state));
}
