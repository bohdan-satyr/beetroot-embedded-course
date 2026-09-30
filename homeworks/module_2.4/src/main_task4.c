#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"


#define BUTTON_GPIO GPIO_NUM_4
#define DEBOUNCE_DELAY_US 50000

typedef enum {
    STATE_IDLE,
    STATE_DEBOUNCE,
    STATE_PRESSED,
    STATE_RELEASE_DEBOUNCE
} button_state_t;

uint32_t counter = 0;

void app_main(void) {
    gpio_reset_pin(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);

    printf("Систему запущено.\n");

    button_state_t current_state = STATE_IDLE;
    int64_t state_timer = 0;

    while (1) {
        int button_level = gpio_get_level(BUTTON_GPIO);
        int64_t current_time = esp_timer_get_time();
        switch (current_state) {
            case STATE_IDLE: {
                if (button_level == 0) {
                    state_timer = current_time;
                    current_state = STATE_DEBOUNCE;
                }
            } break;
            case STATE_DEBOUNCE: {
                if ((current_time - state_timer) > DEBOUNCE_DELAY_US) {
                    if (button_level == 0) {
                        counter++;
                        printf("Успішне натискання! counter = %lu\n", counter);
                        current_state = STATE_PRESSED;
                    } else {                        
                        current_state = STATE_IDLE;
                    }
                }
            }  break;
            case STATE_PRESSED: {
                if (button_level == 1) {
                    state_timer = current_time;
                    current_state = STATE_RELEASE_DEBOUNCE;
                }                
            } break;
            case STATE_RELEASE_DEBOUNCE:{                
                if ((current_time - state_timer) > DEBOUNCE_DELAY_US) {
                    if (button_level == 1) {
                        current_state = STATE_IDLE;
                    } else {
                        current_state = STATE_PRESSED;
                    }
                }
            } break;
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}