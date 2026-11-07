#pragma once
#include "esp_adc/adc_oneshot.h"
#include "hal/adc_types.h"

class Potentiometer {
private:
    adc_oneshot_unit_handle_t adc_handle = nullptr;
    adc_unit_t adc_unit = ADC_UNIT_1;
    adc_channel_t adc_channel;

public:
    void init();
    int readRaw();
    static long map(long x, long in_min, long in_max, long out_min, long out_max);
};
