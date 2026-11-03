#pragma once
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_timer.h"
#include "Pitches.hpp"

class BuzzerPlayer {
private:
    gpio_num_t m_pin;
    ledc_timer_t m_ledc_timer;
    ledc_channel_t m_ledc_channel;
    ledc_mode_t m_ledc_mode;
    
    esp_timer_handle_t m_tick_timer;
    
    const Note* m_current_melody;
    int m_current_note_idx;
    uint32_t m_note_ticks_left;
    bool m_is_playing;

    static constexpr uint32_t TICK_PERIOD_US = 50000;
    static constexpr uint32_t DUTY_50_PERCENT = 512;  

    void set_tone(uint32_t freq);
    void on_tick();
    static void timer_callback_bridge(void* arg);

public:
    BuzzerPlayer(gpio_num_t pin, ledc_timer_t timer = LEDC_TIMER_0, ledc_channel_t channel = LEDC_CHANNEL_0);
    ~BuzzerPlayer();

    bool begin();
    void play(const Note* melody);
    void stop();
    bool is_playing() const;
};
