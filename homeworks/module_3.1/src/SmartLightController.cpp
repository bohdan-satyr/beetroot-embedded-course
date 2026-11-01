#include "SmartLightController.hpp"

SmartLightController::SmartLightController(gpio_num_t led, gpio_num_t ldr, adc_unit_t unit)
    : led_pin(led), ldr_pin(ldr), adc_unit(unit) {}

SmartLightController::~SmartLightController() {
    if (adc_handle) {
        adc_oneshot_del_unit(adc_handle);
    }
}

void SmartLightController::init() {
    // LED config
    gpio_config_t io_conf = {}; 
    io_conf.pin_bit_mask = (1ULL << led_pin);
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);

    // ADC Unit config
    adc_oneshot_unit_init_cfg_t init_config = {}; 
    init_config.unit_id = adc_unit;
    adc_oneshot_new_unit(&init_config, &adc_handle);

    
    adc_unit_t target_unit;
    if (adc_oneshot_io_to_channel(ldr_pin, &target_unit, &ldr_channel) == ESP_OK) {
        adc_oneshot_chan_cfg_t config = {}; 
        
        config.atten = ADC_ATTEN_DB_12;
        config.bitwidth = ADC_BITWIDTH_DEFAULT;
        
        adc_oneshot_config_channel(adc_handle, ldr_channel, &config);
    }
}

bool SmartLightController::read_adc(int &out_raw_value) {
    return adc_oneshot_read(adc_handle, ldr_channel, &out_raw_value) == ESP_OK;
}

void SmartLightController::set_led(bool state) {
    gpio_set_level(led_pin, state ? 1 : 0);
}
