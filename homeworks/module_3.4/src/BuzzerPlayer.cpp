#include "BuzzerPlayer.hpp"
#include "esp_log.h"

BuzzerPlayer::BuzzerPlayer(gpio_num_t pin, ledc_timer_t timer, ledc_channel_t channel)
    : m_pin(pin), m_ledc_timer(timer), m_ledc_channel(channel), 
      m_ledc_mode(LEDC_LOW_SPEED_MODE), m_tick_timer(nullptr),
      m_current_melody(nullptr), m_current_note_idx(0), 
      m_note_ticks_left(0), m_is_playing(false) {}

BuzzerPlayer::~BuzzerPlayer() {
    if (m_tick_timer) {
        esp_timer_stop(m_tick_timer);
        esp_timer_delete(m_tick_timer);
    }
    set_tone(NOTE_PAUSE);
}

void BuzzerPlayer::set_tone(uint32_t freq) {
    if (freq == NOTE_PAUSE) {
        ledc_set_duty(m_ledc_mode, m_ledc_channel, 0);
    } else {
        ledc_set_freq(m_ledc_mode, m_ledc_timer, freq);
        ledc_set_duty(m_ledc_mode, m_ledc_channel, DUTY_50_PERCENT);
    }
    ledc_update_duty(m_ledc_mode, m_ledc_channel);
}

void BuzzerPlayer::timer_callback_bridge(void* arg) {
    static_cast<BuzzerPlayer*>(arg)->on_tick();
}

void BuzzerPlayer::on_tick() {
    if (!m_is_playing || m_current_melody == nullptr) return;

    if (m_note_ticks_left > 0) {
        m_note_ticks_left--;
        return;
    }

    m_current_note_idx++;

    if (m_current_melody[m_current_note_idx].duration_ticks == 0) {
        stop();
        ESP_LOGI("BuzzerPlayer", "Finished playing melody.");
        return;
    }

    const Note& next_note = m_current_melody[m_current_note_idx];
    m_note_ticks_left = next_note.duration_ticks - 1;
    set_tone(next_note.frequency);
}

bool BuzzerPlayer::begin() {
    //Налаштування таймера ШІМ
    ledc_timer_config_t ledc_timer_cfg = {}; 
    ledc_timer_cfg.speed_mode       = m_ledc_mode;
    ledc_timer_cfg.timer_num        = m_ledc_timer;
    ledc_timer_cfg.duty_resolution  = LEDC_TIMER_10_BIT;
    ledc_timer_cfg.freq_hz          = 4000;
    ledc_timer_cfg.clk_cfg          = LEDC_AUTO_CLK;
    
    if (ledc_timer_config(&ledc_timer_cfg) != ESP_OK) return false;

    //Налаштування каналу ШІМ
    ledc_channel_config_t ledc_channel_cfg = {};
    ledc_channel_cfg.speed_mode     = m_ledc_mode;
    ledc_channel_cfg.channel        = m_ledc_channel;
    ledc_channel_cfg.timer_sel      = m_ledc_timer;
    ledc_channel_cfg.intr_type      = LEDC_INTR_DISABLE;
    ledc_channel_cfg.gpio_num       = m_pin;
    ledc_channel_cfg.duty           = 0;
    ledc_channel_cfg.hpoint         = 0;

    if (ledc_channel_config(&ledc_channel_cfg) != ESP_OK) return false;

    // Налаштування періодичного esp_timer
    esp_timer_create_args_t timer_args = {};
    timer_args.callback = &BuzzerPlayer::timer_callback_bridge;
    timer_args.arg = this;
    timer_args.name = "buzzer_tick";
    
    if (esp_timer_create(&timer_args, &m_tick_timer) != ESP_OK) return false;
    if (esp_timer_start_periodic(m_tick_timer, TICK_PERIOD_US) != ESP_OK) return false;

    return true;
}

void BuzzerPlayer::play(const Note* melody) {
    if (melody == nullptr) return;
    
    m_current_melody = melody;
    m_current_note_idx = 0;

    if (m_current_melody[m_current_note_idx].duration_ticks > 0) {
        m_is_playing = true;
        m_note_ticks_left = m_current_melody[m_current_note_idx].duration_ticks - 1;
        set_tone(m_current_melody[m_current_note_idx].frequency);
    }
}

void BuzzerPlayer::stop() {
    m_is_playing = false;
    set_tone(NOTE_PAUSE);
    m_current_melody = nullptr;
}

bool BuzzerPlayer::is_playing() const { 
    return m_is_playing; 
}
