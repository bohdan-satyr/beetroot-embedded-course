#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_rom_sys.h"


#define BUTTON_GPIO GPIO_NUM_4

volatile uint32_t counter = 0;

static void IRAM_ATTR button_isr_handler(void* arg) {
    counter++;
    esp_rom_printf("Interrupt зафіксовано! counter = %lu\n", counter);
}

void app_main(void) {
    gpio_reset_pin(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(0); 
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, NULL);

    printf("Систему запущено. Очікування натискання кнопки...\n");

    while (1) {
        vTaskDelay(1000 / portTICK_PERIOD_MS); 
    }
}