#ifndef WIFI_CONNECT_H
#define WIFI_CONNECT_H

void wifi_event_handler(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data);
void wifi_connection(void);

#endif // WIFI_CONNECT_H
