#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN GPIO_NUM_2  // Set this to your LED GPIO pin if different
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0

TaskHandle_t fade_thread_handler = NULL;
bool stop_fade = 0;


void led_fade(void *pvParameter) {

    int fade_time_ms = *(int *)pvParameter;

    // Configure LED PWM Timer
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = 5000,  // Set PWM frequency to 5 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&ledc_timer);

    // Configure LED PWM Channel
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = LED_PIN,
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .intr_type      = LEDC_INTR_FADE_END,
        .timer_sel      = LEDC_TIMER,
        .duty           = 0,          // Initial duty cycle
        .hpoint         = 0
    };
    ledc_channel_config(&ledc_channel);

    // Enable fade function
    ledc_fade_func_install(0);

    while (1) {
        // Fade in
        ledc_set_fade_time_and_start(LEDC_MODE, LEDC_CHANNEL, 8191, fade_time_ms, LEDC_FADE_NO_WAIT);
        vTaskDelay(pdMS_TO_TICKS(fade_time_ms));

        // Fade out
        ledc_set_fade_time_and_start(LEDC_MODE, LEDC_CHANNEL, 0, fade_time_ms, LEDC_FADE_NO_WAIT);
        vTaskDelay(pdMS_TO_TICKS(fade_time_ms));

        if (stop_fade) {
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, 0);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelete(NULL);
            stop_fade = false;
            break;
        }
    }
}
