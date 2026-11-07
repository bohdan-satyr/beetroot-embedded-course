#include <cstdio>
#include <algorithm>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "Config.hpp"
#include "ServoController.hpp"
#include "Potentiometer.hpp"

static const char* TAG = "MAIN";

extern "C" void app_main(void) {
    ServoController servo;
    Potentiometer pot;

    servo.init();
    pot.init();

    ESP_LOGI(TAG, "System initialized. Starting main loop...");

    int last_angle = -1;

    while (true) {
        int raw_adc = pot.readRaw();
        int current_angle = Potentiometer::map(raw_adc, 0, Config::adcMaxRaw, 0, 180);
        current_angle = std::clamp(current_angle, 0, 180);

        servo.setAngle(current_angle);

        if (current_angle != last_angle) {
            std::printf("Кут відхилення: %d° (ADC: %d)\n", current_angle, raw_adc);
            last_angle = current_angle;
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
