#ifndef LED_H
#define LED_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

extern TaskHandle_t toggle_thread_handler;
void toggle_led_task(void *pvParameter);


#endif // LED_H