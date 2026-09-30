#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"


#define BUTTON_GPIO GPIO_NUM_4
#define DEBOUNCE_DELAY_US 50000

volatile uint32_t counter = 0;
volatile bool is_changed = false;

static void IRAM_ATTR button_isr_handler(void* arg) {
    is_changed = true;
}

void app_main(void) {
    gpio_reset_pin(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0); 
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

    printf("Систему запущено. Очікування натискання кнопки...\n");

    int64_t last_valid_time = 0;

    while (1) {
        if (is_changed){            
            is_changed = false;            

            int64_t current_time = esp_timer_get_time();

            if ((current_time - last_valid_time) > DEBOUNCE_DELAY_US) {
                
                
                if(gpio_get_level(BUTTON_GPIO) == 0) {
                    counter++;

                    printf("Успішне натискання! counter = %lu (пройшло %lld мс від минулого)\n", 
                       counter, (current_time - last_valid_time) / 1000);

                    last_valid_time = current_time;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}