#pragma once

#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

class SmartLightController {
private:
    gpio_num_t led_pin;
    gpio_num_t ldr_pin;
    adc_unit_t adc_unit;
    adc_channel_t ldr_channel;
    adc_oneshot_unit_handle_t adc_handle = nullptr;

public:
    SmartLightController(gpio_num_t led, gpio_num_t ldr, adc_unit_t unit);
    ~SmartLightController();

    void init();
    bool read_adc(int &out_raw_value);
    void set_led(bool state);
};
