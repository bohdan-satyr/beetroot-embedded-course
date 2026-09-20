#pragma once

#include <Arduino.h>

enum class LedState : uint8_t {
    OFF = LOW,
    ON = HIGH
};

class Led {
private:
    const uint8_t _pin;

public:
    explicit Led(uint8_t pin);

    void init() const;
    void set(LedState state) const;
};
