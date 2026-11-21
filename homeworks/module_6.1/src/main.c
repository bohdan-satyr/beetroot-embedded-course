#include <stdio.h>
#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define RED_LED_GPIO    GPIO_NUM_5
#define YELLOW_LED_GPIO GPIO_NUM_6
#define GREEN_LED_GPIO  GPIO_NUM_7

typedef struct {
    gpio_num_t pin;
    uint32_t interval_ms;
} led_config_t;

static led_config_t red_led_cfg    = { .pin = RED_LED_GPIO,    .interval_ms = 200 };
static led_config_t yellow_led_cfg = { .pin = YELLOW_LED_GPIO, .interval_ms = 500 };
static led_config_t green_led_cfg  = { .pin = GREEN_LED_GPIO,  .interval_ms = 1000 };

void led_blink_task(void *pvParameters) {
    led_config_t *cfg = (led_config_t *)pvParameters;

    gpio_reset_pin(cfg->pin);
    gpio_set_direction(cfg->pin, GPIO_MODE_INPUT_OUTPUT);

    while (1) {
        gpio_set_level(cfg->pin, !gpio_get_level(cfg->pin));   
        vTaskDelay(pdMS_TO_TICKS(cfg->interval_ms));
    }
}

void app_main(void) {
    xTaskCreate(led_blink_task, "Red_LED_Task",    2048, &red_led_cfg,    5, NULL);
    xTaskCreate(led_blink_task, "Yellow_LED_Task", 2048, &yellow_led_cfg, 5, NULL);
    xTaskCreate(led_blink_task, "Green_LED_Task",  2048, &green_led_cfg,  5, NULL);
}
