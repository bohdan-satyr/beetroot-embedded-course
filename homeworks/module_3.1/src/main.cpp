#include <cstdio>
#include <unistd.h>
#include "esp_log.h"
#include "SmartLightController.hpp"
#include "SmaFilter.hpp"

static const char *TAG = "MAIN";

constexpr gpio_num_t LED_PIN = GPIO_NUM_5;
constexpr gpio_num_t LDR_PIN = GPIO_NUM_4;
constexpr adc_unit_t ADC_UNIT = ADC_UNIT_1;

constexpr int32_t THRESHOLD_DARK = 1500;
constexpr int32_t THRESHOLD_LIGHT = 2000;
constexpr size_t SMA_WINDOW = 10;

extern "C" void app_main(void)
{
    SmartLightController controller(LED_PIN, LDR_PIN, ADC_UNIT);
    SmaFilter filter(SMA_WINDOW);

    controller.init();

    bool led_state = false;

    while (true) {
        int raw_value = 0;

        if (controller.read_adc(raw_value)) {
            int32_t filtered_value = filter.process(raw_value);

            if (filtered_value < THRESHOLD_DARK && !led_state) {
                led_state = true;
                controller.set_led(led_state);
                ESP_LOGW(TAG, "ТЕМНО -> LED ON. (ADC: %ld)", filtered_value);
            }
            else if (filtered_value > THRESHOLD_LIGHT && led_state) {
                led_state = false;
                controller.set_led(led_state);
                ESP_LOGI(TAG, "СВІТЛО -> LED OFF. (ADC: %ld)", filtered_value);
            }

            ESP_LOGI(TAG, "Raw: %d | Filtered: %ld | LED: %s", raw_value, filtered_value, led_state ? "ON" : "OFF");
        } else {
            ESP_LOGE(TAG, "ADC Error!");
        }

        usleep(50000);
    }
}
