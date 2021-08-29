#ifndef __MAIN_H
#define __MAIN_H

#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "nvs.h"
#include "nvs_flash.h"

#include "esp_system.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "freertos/event_groups.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc_cal.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_blufi_api.h"
#include "esp_bt_defs.h"
#include "esp_gap_ble_api.h"
#include "esp_bt_main.h"
#include "esp_bt_device.h"
#include "esp_websocket_client.h"

#include "mbedtls/aes.h"
#include "mbedtls/dhm.h"
#include "mbedtls/md5.h"
#include "esp_crc.h"

#include "gattc.h"
#include "irsensor.h"
#include "deepsleep.h"
#include "misc.h"
#include "blufi.h"
#include "websocket.h"

bool main_wifi_ready(void);
uint8_t * main_get_ssid(void);
uint8_t * main_get_password(void);
void main_get_wifi(wifi_config_t * config);
void main_set_wifi(wifi_config_t * config);

#endif /* __MAIN_H */