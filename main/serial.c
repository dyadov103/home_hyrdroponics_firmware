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
#include "serial.h"

void gen_serial() {
    unsigned char mac_base[6] = {0};
    esp_read_mac(mac_base, ESP_MAC_WIFI_STA);

    // Print the MAC address
    printf("MAC Address: %02X:%02X:%02X:%02X:%02X:%02X\n", 
           mac_base[0], mac_base[1], mac_base[2], 
           mac_base[3], mac_base[4], mac_base[5]);

    // Generate a unique numeric serial number based on the MAC address
    unsigned long long serial_num = 0;
    for (int i = 0; i < 6; i++) {
        serial_num = serial_num * 256 + mac_base[i];
    }

    // Format as a numeric string
    char serial[21]; // Max length for 64-bit integer + null terminator
    snprintf(serial, sizeof(serial), "%llu", serial_num);

    // Print the unique numeric serial number
    printf("Unique Serial (Numeric): %s\n", serial);
}