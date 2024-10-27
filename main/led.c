#include <stdio.h> //for basic printf commands
#include <string.h> //for handling strings
#include "freertos/FreeRTOS.h" //for delay,mutexs,semphrs rtos operations
#include "esp_system.h" //esp_init funtions esp_err_t 
#include "esp_wifi.h" //esp_wifi_init functions and wifi operations
#include "esp_log.h" //for showing logs
#include "esp_event.h" //for wifi event
#include "esp_mac.h"   // For ESP_MAC_WIFI_STA
#include "nvs_flash.h" //non volatile storage
#include "lwip/err.h" //light weight ip packets error handling
#include "lwip/sys.h" //system applications for light weight ip apps
#include "driver/gpio.h"
#include "wifi.h"
#include "led.h"
#include "definitions.h"

TaskHandle_t toggle_thread_handler = NULL; // Task handle for the LED toggle task

void toggle_led_task(void *pvParameter) {
    // Configure the LED_PIN as an output GPIO
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    int led_state = 0;

    while (1) {
        // Toggle the LED state
        led_state = !led_state;
        printf("toggling led %d\n\n", led_state);
        gpio_set_level(LED_PIN, !led_state);

        // Wait for 500 ms
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}