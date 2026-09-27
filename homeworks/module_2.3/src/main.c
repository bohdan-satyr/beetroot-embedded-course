#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"


#define RED_LED_GPIO    GPIO_NUM_5
#define YELLOW_LED_GPIO GPIO_NUM_6
#define GREEN_LED_GPIO  GPIO_NUM_7

uint64_t storedTimeStampRed = 0;
uint64_t storedTimeStampYellow = 0;
uint64_t storedTimeStampGreen = 0;

const int intervalRed = 200;
const int intervalYellow = 500;
const int intervalGreen = 1000;

void app_main(void) {
    gpio_reset_pin(RED_LED_GPIO);
    gpio_reset_pin(YELLOW_LED_GPIO);
    gpio_reset_pin(GREEN_LED_GPIO);

    gpio_set_direction(RED_LED_GPIO, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(YELLOW_LED_GPIO, GPIO_MODE_INPUT_OUTPUT);
    gpio_set_direction(GREEN_LED_GPIO, GPIO_MODE_INPUT_OUTPUT);

    
    while (1) {
        uint64_t currentMillis = esp_timer_get_time() / 1000;
        if (currentMillis - storedTimeStampRed >= intervalRed) {
            storedTimeStampRed = currentMillis;
            gpio_set_level(RED_LED_GPIO, !gpio_get_level(RED_LED_GPIO));
        }

        if (currentMillis - storedTimeStampYellow >= intervalYellow) {
            storedTimeStampYellow = currentMillis;
            gpio_set_level(YELLOW_LED_GPIO, !gpio_get_level(YELLOW_LED_GPIO));
        }

        if (currentMillis - storedTimeStampGreen >= intervalGreen) {
            storedTimeStampGreen = currentMillis;
            gpio_set_level(GREEN_LED_GPIO, !gpio_get_level(GREEN_LED_GPIO));
        }

        vTaskDelay(1); 
    }
}