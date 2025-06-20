#ifndef LEDC_PWM_H
#define LEDC_PWM_H

#include "driver/ledc.h"
#include <stdint.h>

class LedcPwm {
private:
    ledc_channel_t channel;
    ledc_timer_t timer;
    int gpioNum;
    int frequency;
    static constexpr int resolution = 16; // 16-bit resolution
public:
    LedcPwm(ledc_channel_t channel, ledc_timer_t timer, int gpioNum, int frequency = 330)
        : channel(channel), timer(timer), gpioNum(gpioNum), frequency(frequency) {
        ledc_timer_config_t timerConfig = {
            .duty_resolution = LEDC_TIMER_16_BIT,
            .freq_hz = frequency,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_num = timer
        };
        ledc_timer_config(&timerConfig);

        ledc_channel_config_t channelConfig = {
            .channel = channel,
            .duty = 0,
            .gpio_num = gpioNum,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .timer_sel = timer
        };
        ledc_channel_config(&channelConfig);
    }

    ~LedcPwm() {
        ledc_stop(LEDC_HIGH_SPEED_MODE, channel, 0);
    }

    void setDutyCycle(uint16_t duty) {
        if (duty > (1 << resolution) - 1) duty = (1 << resolution) - 1;
        ledc_set_duty(LEDC_HIGH_SPEED_MODE, channel, duty);
        ledc_update_duty(LEDC_HIGH_SPEED_MODE, channel);
    }
};

#endif