#ifndef LED_H
#define LED_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern bool stop_fade;
extern TaskHandle_t fade_thread_handler;
void led_fade(void *pvParameter);


#endif // LED_H