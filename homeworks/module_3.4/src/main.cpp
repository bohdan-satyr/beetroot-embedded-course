#include <cstdio>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "BuzzerPlayer.hpp"
#include "Melodies.hpp"

#define BUZZER_PIN   GPIO_NUM_4  

const Note* const playlist[] = {
    Melodies::mario,
    Melodies::win,
    Melodies::error,
    Melodies::sigma,
    Melodies::gigachad,
    Melodies::chipi
};

const char* const playlist_names[] = {
    "Mario",
    "Success Sound",
    "Failed",
    "Sigma Face",
    "GigaChad Theme",
    "Chipi Chipi Chapa Chapa"
};

constexpr int TOTAL_MELODIES = sizeof(playlist) / sizeof(playlist[0]);

extern "C" void app_main(void) {
    BuzzerPlayer player(BUZZER_PIN);

    if (!player.begin()) {
        ESP_LOGE("Main", "Buzzer Failed to initialize");
        return;
    }

    int current_track_idx = 0;
    bool first_run = true;

    ESP_LOGI("Main", "Player ON!");

    while (true) {
        if (!player.is_playing()) {
            if (!first_run) {
                current_track_idx = (current_track_idx + 1) % TOTAL_MELODIES;
                vTaskDelay(pdMS_TO_TICKS(1500));
            } else {
                first_run = false;
            }

            ESP_LOGI("Main", "Playing track [%d]: %s", current_track_idx, playlist_names[current_track_idx]);
            player.play(playlist[current_track_idx]);
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
