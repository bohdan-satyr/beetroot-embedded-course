#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"

#define LED_PIN             9
#define MOTOR_PIN           10
#define LEDC_TIMER          LEDC_TIMER_0
#define LEDC_FREQUENCY      1000       // 1 кГц
#define LEDC_RESOLUTION     LEDC_TIMER_10_BIT // 10-bit (0-1023)

#define LED_CHANNEL         LEDC_CHANNEL_0
#define MOTOR_CHANNEL       LEDC_CHANNEL_1

#define ADC_POT_CHANNEL     ADC_CHANNEL_3 // GPIO 4

#define BUTTON_BOOT_PIN     GPIO_NUM_0

adc_oneshot_unit_handle_t adc1_handle;

typedef enum {
    MODE_CONTROL_LED,
    MODE_CONTROL_MOTOR
} control_mode_t;

control_mode_t current_mode = MODE_CONTROL_LED;

#define LEDC_FREQUENCY_LED   1000  // 1 кГц 
#define LEDC_FREQUENCY_MOTOR 20000 // 20 кГц

void init_pwm() {
    // Налаштування ТАЙМЕРА 0 для світлодіода
    ledc_timer_config_t timer_led = {};
    timer_led.speed_mode = LEDC_LOW_SPEED_MODE;
    timer_led.duty_resolution = LEDC_RESOLUTION;
    timer_led.timer_num = LEDC_TIMER_0;        // Таймер 0
    timer_led.freq_hz = LEDC_FREQUENCY_LED;
    timer_led.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timer_led);

    // Налаштування ТАЙМЕРА 1 для двигуна
    ledc_timer_config_t timer_motor = {};
    timer_motor.speed_mode = LEDC_LOW_SPEED_MODE;
    timer_motor.duty_resolution = LEDC_RESOLUTION;
    timer_motor.timer_num = LEDC_TIMER_1;     // Таймер 1
    timer_motor.freq_hz = LEDC_FREQUENCY_MOTOR;
    timer_motor.clk_cfg = LEDC_AUTO_CLK;
    ledc_timer_config(&timer_motor);

    // Конфігурація каналу для світлодіода (прив'язаний до LEDC_TIMER_0)
    ledc_channel_config_t led_channel_cfg = {};
    led_channel_cfg.gpio_num = LED_PIN;
    led_channel_cfg.speed_mode = LEDC_LOW_SPEED_MODE;
    led_channel_cfg.channel = LED_CHANNEL;
    led_channel_cfg.timer_sel = LEDC_TIMER_0; // Використовує Таймер 0
    led_channel_cfg.duty = 0;
    led_channel_cfg.hpoint = 0;
    ledc_channel_config(&led_channel_cfg);

    // Конфігурація каналу для двигуна (прив'язаний до LEDC_TIMER_1)
    ledc_channel_config_t motor_channel_cfg = {};
    motor_channel_cfg.gpio_num = MOTOR_PIN;
    motor_channel_cfg.speed_mode = LEDC_LOW_SPEED_MODE;
    motor_channel_cfg.channel = MOTOR_CHANNEL;
    motor_channel_cfg.timer_sel = LEDC_TIMER_1; // Використовує Таймер 1
    motor_channel_cfg.duty = 0;
    motor_channel_cfg.hpoint = 0;
    ledc_channel_config(&motor_channel_cfg);
}


void init_adc() {
    adc_oneshot_unit_init_cfg_t init_config1 = {};
    init_config1.unit_id = ADC_UNIT_1;
    init_config1.clk_src = ADC_RTC_CLK_SRC_DEFAULT;
    init_config1.ulp_mode = ADC_ULP_MODE_DISABLE;
    adc_oneshot_new_unit(&init_config1, &adc1_handle);

    adc_oneshot_chan_cfg_t config = {};
    config.atten = ADC_ATTEN_DB_12;
    config.bitwidth = ADC_BITWIDTH_DEFAULT;
    adc_oneshot_config_channel(adc1_handle, ADC_POT_CHANNEL, &config);
}

void init_button() {
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << BUTTON_BOOT_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    gpio_config(&io_conf);
}

extern "C" void app_main(void) {
    init_pwm();
    init_adc();
    init_button();

    int adc_raw = 0;
    bool last_button_state = true;

    while (true) {
        bool current_button_state = gpio_get_level(BUTTON_BOOT_PIN);
        
        if (last_button_state == true && current_button_state == false) {
            vTaskDelay(pdMS_TO_TICKS(50));
            if (gpio_get_level(BUTTON_BOOT_PIN) == false) {
                if (current_mode == MODE_CONTROL_LED) {
                    current_mode = MODE_CONTROL_MOTOR;
                    printf("Motor mode\n");
                } else {
                    current_mode = MODE_CONTROL_LED;
                    printf("LED mode\n");
                }
            }
        }
        last_button_state = current_button_state;

        adc_oneshot_read(adc1_handle, ADC_POT_CHANNEL, &adc_raw);
        int duty_value = adc_raw >> 2; // Зсув до 10 біт (0...1023)

        if (current_mode == MODE_CONTROL_LED) {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LED_CHANNEL, duty_value);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LED_CHANNEL);
        } else {
            ledc_set_duty(LEDC_LOW_SPEED_MODE, MOTOR_CHANNEL, duty_value);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, MOTOR_CHANNEL);
        }

        vTaskDelay(pdMS_TO_TICKS(20));
    }
}