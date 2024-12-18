// ble_server.c

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_bt_main.h"
#include "esp_bt.h"
#include "bluetooth.h"

#define GATTS_TAG "BLE_SERVER"
#define ESP_APP_ID 0x55
#define SVC_INST_ID 0

// UUIDs for service and characteristic
static uint8_t service_uuid[16] = {
    0x00, 0x00, 0xAB, 0xCD, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x78
};
static uint8_t char_uuid[16] = {
    0x00, 0x00, 0xAB, 0xCD, 0x12, 0x34, 0x56, 0x78, 0x90, 0xAB, 0xCD, 0xEF, 0x12, 0x34, 0x56, 0x79
};

// GATT server handles
static uint16_t gatts_handle_table[1];

// GATT Event Handler
static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param) {
    switch (event) {
        case ESP_GATTS_WRITE_EVT:
            if (param->write.handle == gatts_handle_table[0]) {
                // Received a message
                char received_data[100] = {0};  // Buffer for received data
                memcpy(received_data, param->write.value, param->write.len);
                printf("Received data: %s\n", received_data);
            }
            break;
        case ESP_GATTS_ADD_CHAR_EVT:
            gatts_handle_table[0] = param->add_char.attr_handle;
            break;
        default:
            break;
    }
}

// Initialize BLE Server
void ble_server_init(void) {
    // Initialize Bluetooth stack
    esp_err_t ret = esp_bluedroid_init();
    if (ret) {
        printf("Bluetooth init failed: %s\n", esp_err_to_name(ret));
        return;
    }
    ret = esp_bluedroid_enable();
    if (ret) {
        printf("Bluetooth enable failed: %s\n", esp_err_to_name(ret));
        return;
    }

    // Register GATT server callback and app
    esp_ble_gatts_register_callback(gatts_event_handler);
    esp_ble_gatts_app_register(ESP_APP_ID);

    // Create BLE service and add characteristic
    esp_ble_gatts_create_attr_tab(esp_gatts_attr_db_t[]){
        {ESP_GATT_AUTO_RSP, ESP_UUID_LEN_16, (uint8_t *)service_uuid, ESP_GATT_PERM_READ | ESP_GATT_PERM_WRITE},
        {ESP_GATT_AUTO_RSP, ESP_UUID_LEN_16, (uint8_t *)char_uuid, ESP_GATT_PERM_WRITE, NULL, 100}  // Writable characteristic
    }, ESP_GATT_IF_NONE, 2, SVC_INST_ID);
}
