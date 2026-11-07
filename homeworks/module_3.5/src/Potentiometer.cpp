#include "Potentiometer.hpp"
#include "Config.hpp"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

void Potentiometer::init() {
    // Вираховуємо номер каналу АЦП та юніту за номером GPIO
    ESP_ERROR_CHECK(adc_oneshot_io_to_channel(Config::potGpioPin, &adc_unit, &adc_channel));

    // Конфігурація модуля АЦП (встановлюємо clk_src в 0 для автовибору)
    adc_oneshot_unit_init_cfg_t init_config = {};
    init_config.unit_id = adc_unit;
    init_config.clk_src = static_cast<adc_oneshot_clk_src_t>(0); // Автоматичний вибір тактування
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));

    // Конфігурація каналу АЦП
    adc_oneshot_chan_cfg_t channel_config = {};
    channel_config.bitwidth = ADC_BITWIDTH_12;
    channel_config.atten = ADC_ATTEN_DB_12;
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, adc_channel, &channel_config));
    
    ESP_LOGI("POT", "Потенціометр ініціалізовано на GPIO %d (Unit %d, Channel %d)", 
             Config::potGpioPin, adc_unit, adc_channel);
}

int Potentiometer::readRaw() {
    int raw_value = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, adc_channel, &raw_value));
    return raw_value;
}

long Potentiometer::map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
