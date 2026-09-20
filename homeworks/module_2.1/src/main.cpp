#include <Arduino.h>
#include "Led.h"
#include "Config.h"

enum class LedMode : uint8_t {
    BLINKING,
	ALWAYS_ON,
    ALWAYS_OFF
};

Led myLed(Config::Led::pin);

volatile bool buttonPressed = false;

void IRAM_ATTR handleButtonInterrupt() {
    buttonPressed = true;
}

void setup() {
    Serial.begin(Config::Monitor::baudRate);
    myLed.init();

    pinMode(Config::Button::pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(Config::Button::pin), handleButtonInterrupt, FALLING);
}

void loop() {
    static unsigned long lastDebounceTime = 0;
    static LedMode currentMode = LedMode::BLINKING;
    
    if (buttonPressed) {
        buttonPressed = false;

        unsigned long currentMillis = millis();
        if (currentMillis - lastDebounceTime > Config::Button::debounceDelay) {
          lastDebounceTime = currentMillis;

          if (currentMode == LedMode::BLINKING) {
              currentMode = LedMode::ALWAYS_ON;
              Serial.println("Mode changed to: Always ON");
          } else if (currentMode == LedMode::ALWAYS_ON) {
              currentMode = LedMode::ALWAYS_OFF;
              Serial.println("Mode changed to: Always OFF");
          } else {
              currentMode = LedMode::BLINKING;
              Serial.println("Mode changed to: Blinking");
          }
        }
    }

    static unsigned long previousLedMillis = 0;
    static LedState currentLedState = LedState::OFF;

    unsigned long currentMillis = millis();

    switch (currentMode) {
      
        case LedMode::BLINKING:
            if (currentMillis - previousLedMillis >= Config::Led::blinkInterval) {
                previousLedMillis = currentMillis;
                currentLedState = (currentLedState == LedState::OFF) ? LedState::ON : LedState::OFF;
                myLed.set(currentLedState);
            }
            break;

        case LedMode::ALWAYS_ON:
            myLed.set(LedState::ON);
            break;

        case LedMode::ALWAYS_OFF:
            myLed.set(LedState::OFF);
            break;
    }

    static unsigned int loopCounter = 0;
    static unsigned long startTimeMicros = 0;

    if (loopCounter == 0) {
        startTimeMicros = micros();
    }
    loopCounter++;

    if (loopCounter >= Config::Monitor::iterationsTarget) {
        unsigned long endTimeMicros = micros();
        unsigned long totalDuration = endTimeMicros - startTimeMicros;
        float averageLoopTime = static_cast<float>(totalDuration) / loopCounter;

        Serial.printf("Average loop() time over 1000 runs: %.3f microseconds\n", averageLoopTime);
        
        loopCounter = 0;
    }
}