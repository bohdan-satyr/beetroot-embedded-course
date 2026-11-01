#include <cstdio>
#include <cmath>
#include <cstring>
#include <unistd.h>
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"

constexpr gpio_num_t POT_PIN = GPIO_NUM_4;
constexpr adc_unit_t ADC_UNIT = ADC_UNIT_1;

constexpr float VMAX_MV = 3100.0f;        // Максимальна вимірювана напруга при ATTEN_DB_12 (3.1 В)
constexpr float ADC_MAX_VAL = 4095.0f;    // 12-бітна розрядність (2^12 - 1)
constexpr const char* ATTEN_STR = "12 dB";// Рівень атенюації
constexpr float VREF_MANUAL = 1100.0f;    // Базова опорна напруга АЦП ESP32 (~1.1 В внутрішня)

extern "C" void app_main(void) {
    adc_oneshot_unit_handle_t adc_handle = nullptr;
    adc_oneshot_unit_init_cfg_t init_config;
    std::memset(&init_config, 0, sizeof(init_config));
    init_config.unit_id = ADC_UNIT;
    adc_oneshot_new_unit(&init_config, &adc_handle);

    adc_channel_t adc_channel;
    adc_unit_t target_unit;
    if (adc_oneshot_io_to_channel(POT_PIN, &target_unit, &adc_channel) != ESP_OK) {
        printf("Помилка: GPIO %d не підтримує функцію ADC!\n", POT_PIN);
        return;
    }

    adc_oneshot_chan_cfg_t chan_config;
    std::memset(&chan_config, 0, sizeof(chan_config));
    chan_config.atten = ADC_ATTEN_DB_12;
    chan_config.bitwidth = ADC_BITWIDTH_DEFAULT;
    adc_oneshot_config_channel(adc_handle, adc_channel, &chan_config);

    adc_cali_handle_t cali_handle = nullptr;
    bool cali_enabled = false;

    adc_cali_curve_fitting_config_t cali_config; 
    std::memset(&cali_config, 0, sizeof(cali_config));
    cali_config.unit_id = ADC_UNIT;
    cali_config.chan = adc_channel;
    cali_config.atten = ADC_ATTEN_DB_12;
    cali_config.bitwidth = ADC_BITWIDTH_DEFAULT;

    if (adc_cali_create_scheme_curve_fitting(&cali_config, &cali_handle) == ESP_OK) {
        cali_enabled = true;
    }

    printf("%-7s %-14s %-12s %-8s\n", "RAW", "U_manual(mV)", "U_cali(mV)", "Error(%)");
    printf("--------------------------------------------------\n");

    while (true) {
        int raw_value = 0;
        int u_cali_mv = 0;

        if (adc_oneshot_read(adc_handle, adc_channel, &raw_value) == ESP_OK) {
            
            float u_manual_mv = ((float)raw_value * VMAX_MV) / ADC_MAX_VAL;

            if (cali_enabled) {
                adc_cali_raw_to_voltage(cali_handle, raw_value, &u_cali_mv);
            } else {
                u_cali_mv = 0;
            }

            float error_percent = 0.0f;
            if (u_cali_mv > 0) {
                error_percent = (std::fabs(u_manual_mv - (float)u_cali_mv) / (float)u_cali_mv) * 100.0f;
            }

            printf("%-7d %-14.1f %-12d %.2f\n", raw_value, u_manual_mv, u_cali_mv, error_percent);
        }

        usleep(100000);
    }
}
