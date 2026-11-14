#include <cstring>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "driver/rmt_tx.h"
#include "esp_log.h"

constexpr gpio_num_t BTN_PIN = GPIO_NUM_0;
constexpr gpio_num_t LED_PIN = GPIO_NUM_48; 
constexpr uart_port_t UART_NUM = UART_NUM_1;
constexpr int TXD_PIN = 17;                  // Підключити до PA3 (RX STM32)
constexpr int RXD_PIN = 18;                  // Підключити до PA2 (TX STM32)
constexpr int BUF_SIZE = 1024;

static const char* TAG = "ESP32_UART_TEST";

class RgbLed {
private:
    rmt_channel_handle_t led_chan = nullptr;
    rmt_encoder_handle_t simple_encoder = nullptr;
    bool state = false;

public:
    void init(gpio_num_t pin) {
        rmt_tx_channel_config_t tx_chan_config = {};
        tx_chan_config.clk_src = RMT_CLK_SRC_DEFAULT;
        tx_chan_config.gpio_num = pin;
        tx_chan_config.mem_block_symbols = 64;
        tx_chan_config.resolution_hz = 10000000; // 10 МГц (1 тік = 100 нс)
        tx_chan_config.trans_queue_depth = 4;
        ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

        rmt_bytes_encoder_config_t bytes_encoder_config = {};
        // Таймінги (0: 400нс high / 850нс low, 1: 800нс high / 450нс low)
        bytes_encoder_config.bit0.level0 = 1;
        bytes_encoder_config.bit0.duration0 = 4;
        bytes_encoder_config.bit0.level1 = 0;
        bytes_encoder_config.bit0.duration1 = 8;
        bytes_encoder_config.bit1.level0 = 1;
        bytes_encoder_config.bit1.duration0 = 8;
        bytes_encoder_config.bit1.level1 = 0;
        bytes_encoder_config.bit1.duration1 = 5;
        bytes_encoder_config.flags.msb_first = 1;
        ESP_ERROR_CHECK(rmt_new_bytes_encoder(&bytes_encoder_config, &simple_encoder));

        ESP_ERROR_CHECK(rmt_enable(led_chan));
        setColor(0, 0, 0);
    }

    void setColor(uint8_t r, uint8_t g, uint8_t b) {
        uint8_t grb[3] = {g, r, b}; // WS2812 приймає байти в порядку Green-Red-Blue
        rmt_transmit_config_t tx_config = {};
        tx_config.loop_count = 0;
        rmt_transmit(led_chan, simple_encoder, grb, sizeof(grb), &tx_config);
        rmt_tx_wait_all_done(led_chan, portMAX_DELAY);
    }

    void toggle() {
        state = !state;
        if (state) {
            setColor(0, 40, 0);
        } else {
            setColor(0, 0, 0);
        }
    }
};

extern "C" void app_main(void) {
    //Налаштування UART1
    uart_config_t uart_config = {};
    uart_config.baud_rate = 115200;
    uart_config.data_bits = UART_DATA_8_BITS;
    uart_config.parity = UART_PARITY_DISABLE;
    uart_config.stop_bits = UART_STOP_BITS_1;
    uart_config.flow_ctrl = UART_HW_FLOWCTRL_DISABLE;
    uart_config.source_clk = UART_SCLK_DEFAULT;

    ESP_ERROR_CHECK(uart_driver_install(UART_NUM, BUF_SIZE * 2, 0, 0, nullptr, 0));
    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    //Налаштування кнопки BOOT (GPIO0)
    gpio_config_t io_conf = {};
    io_conf.pin_bit_mask = (1ULL << BTN_PIN);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    //Ініціалізація RGB-діода
    RgbLed led;
    led.init(LED_PIN);

    ESP_LOGI(TAG, "ESP32-S3 (ESP-IDF C++) UART Test Started!");

    int last_btn_state = 1;
    uint8_t rx_byte = 0;
    const char tx_cmd = 'T';

    while (true) {
        int current_btn_state = gpio_get_level(BTN_PIN);
        if (last_btn_state == 1 && current_btn_state == 0) {
            vTaskDelay(pdMS_TO_TICKS(50));
            if (gpio_get_level(BTN_PIN) == 0) {
                uart_write_bytes(UART_NUM, &tx_cmd, 1);
                ESP_LOGI(TAG, "Button pressed -> Sent 'T' to STM32");
            }
        }
        last_btn_state = current_btn_state;

        int len = uart_read_bytes(UART_NUM, &rx_byte, 1, pdMS_TO_TICKS(10));
        if (len > 0 && rx_byte == 'T') {
            led.toggle();
            ESP_LOGI(TAG, "Received 'T' from STM32 -> LED toggled");
        }
    }
}
