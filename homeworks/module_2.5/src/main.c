#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "led_strip.h"

#define RELAY_PIN GPIO_NUM_4
#define RGB_LED_PIN GPIO_NUM_48

const uint64_t MOTOR_RUN_TIME_US = 15 * 60 * 1000 * 1000ULL; // 15 min
const uint64_t MOTOR_REST_TIME_US = 45 * 60 * 1000 * 1000ULL; // 45 min

static const char *TAG = "TIMER_CONTROL";
static led_strip_handle_t led_strip;
static esp_timer_handle_t motor_timer;

static bool is_motor_running = false;

static void motor_timer_callback(void* arg)
{
    is_motor_running = !is_motor_running;

    if (is_motor_running) {
        ESP_LOGI(TAG, "Таймер: Вмикаємо двигун (Зелений світлодіод)");
        
        gpio_set_level(RELAY_PIN, 1);
        led_strip_set_pixel(led_strip, 0, 0, 50, 0); // GREEN
        led_strip_refresh(led_strip);

        ESP_ERROR_CHECK(esp_timer_start_once(motor_timer, MOTOR_RUN_TIME_US));
    } 
    else {
        ESP_LOGI(TAG, "Таймер: Вимикаємо двигун (Червоний світлодіод)");
        
        gpio_set_level(RELAY_PIN, 0);
        led_strip_set_pixel(led_strip, 0, 50, 0, 0); // RED
        led_strip_refresh(led_strip);

        ESP_ERROR_CHECK(esp_timer_start_once(motor_timer, MOTOR_REST_TIME_US));
    }
}

void app_main(void) {
    gpio_reset_pin(RELAY_PIN);
    gpio_set_direction(RELAY_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(RELAY_PIN, 0);

    //for RGB
    led_strip_config_t strip_config = { .strip_gpio_num = RGB_LED_PIN, .max_leds = 1 };
    led_strip_rmt_config_t rmt_config = { .resolution_hz = 10 * 1000 * 1000, .flags.with_dma = false };
    ESP_ERROR_CHECK(led_strip_new_rmt_device(&strip_config, &rmt_config, &led_strip));

    led_strip_set_pixel(led_strip, 0, 50, 0, 0);
    led_strip_refresh(led_strip);

    //configure timer
    const esp_timer_create_args_t timer_args = {
        .callback = &motor_timer_callback,
        .name = "motor_timer"
    };

    //create timer
    ESP_ERROR_CHECK(esp_timer_create(&timer_args, &motor_timer));

    ESP_LOGI(TAG, "Старт системи. Двигун відпочиває.");
    ESP_ERROR_CHECK(esp_timer_start_once(motor_timer, MOTOR_REST_TIME_US));
}