#include "ServoController.hpp"
#include "Config.hpp"
#include "driver/ledc.h"
#include <algorithm>

void ServoController::init() {
    ledc_timer_config_t ledc_timer = {};
    ledc_timer.speed_mode       = Config::ledcMode;
    ledc_timer.duty_resolution  = Config::dutyRes;
    ledc_timer.timer_num        = Config::ledcTimer;
    ledc_timer.freq_hz          = Config::servoFreq;
    ledc_timer.clk_cfg          = LEDC_AUTO_CLK;
    ledc_timer_config(&ledc_timer);

    ledc_channel_config_t ledc_channel = {};
    ledc_channel.speed_mode     = Config::ledcMode;
    ledc_channel.channel        = Config::ledcChannel;
    ledc_channel.timer_sel      = Config::ledcTimer;
    ledc_channel.gpio_num       = Config::servoPin;
    ledc_channel.duty           = 0;
    ledc_channel.hpoint         = 0;
    
    ledc_channel_config(&ledc_channel);
}

void ServoController::setAngle(int angle) {
    angle = std::clamp(angle, 0, 180);
    uint32_t us = Config::servoMinUs + ((Config::servoMaxUs - Config::servoMinUs) * angle / 180);
    uint32_t duty = (us * Config::maxDuty) / 20000;

    ledc_set_duty(Config::ledcMode, Config::ledcChannel, duty);
    ledc_update_duty(Config::ledcMode, Config::ledcChannel);
}
