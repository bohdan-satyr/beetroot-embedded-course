#pragma once
#include <Arduino.h>

struct Config {
    struct Led {
        static constexpr uint8_t pin = 15;
        static constexpr unsigned long blinkInterval = 500;
    };

    struct Button {
        static constexpr uint8_t pin = 5;  
        static constexpr unsigned long debounceDelay = 50;
    };

    struct Monitor {
        static constexpr unsigned long baudRate = 115200;
        static constexpr unsigned long iterationsTarget = 1000;
    };
};
