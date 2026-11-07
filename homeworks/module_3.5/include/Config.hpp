#pragma once
#include "driver/ledc.h"
#include "hal/adc_types.h"

namespace Config {
    constexpr int potGpioPin = 1; 
    constexpr int adcMaxRaw = 4095;

    constexpr int servoPin = 18;
    constexpr uint32_t servoFreq = 50;
    constexpr uint32_t servoMinUs = 500;
    constexpr uint32_t servoMaxUs = 2400;
    constexpr uint32_t maxDuty = 16383;

    constexpr ledc_mode_t ledcMode = LEDC_LOW_SPEED_MODE;
    constexpr ledc_channel_t ledcChannel = LEDC_CHANNEL_0;
    constexpr ledc_timer_t ledcTimer = LEDC_TIMER_0;
    constexpr ledc_timer_bit_t dutyRes = LEDC_TIMER_14_BIT;

}
