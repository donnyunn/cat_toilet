/*
   This code is written by Dulab Ltd. but it is in the Public Domain 
   (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

/****************************************************************************
*
* This file is a main code for the cat toilet module.
*
****************************************************************************/

#include "main.h"

#define TAG "MAIN"

#define CNT_WAIT_CONNECT 60
#define CNT_IR_CHECK 5
#define CNT_WS_RETRY 5

RTC_DATA_ATTR wifi_config_t wifi_info;
RTC_DATA_ATTR bool wifi_ready = false;
RTC_DATA_ATTR bool power;

bool main_wifi_ready(void)
{
    return wifi_ready;
}

uint8_t * main_get_ssid(void)
{
    return wifi_info.sta.ssid;
}

uint8_t * main_get_password(void)
{
    return wifi_info.sta.password;
}

void main_get_wifi(wifi_config_t * config)
{
    memcpy(config->sta.ssid, wifi_info.sta.ssid, sizeof(wifi_info.sta.ssid));
    memcpy(config->sta.password, wifi_info.sta.password, sizeof(wifi_info.sta.password));
    printf("%s\n", wifi_info.sta.ssid);
}

void main_set_wifi(wifi_config_t * config)
{
    memcpy(&wifi_info, config, sizeof(wifi_config_t));
    wifi_ready = true;
    printf("%s\n", wifi_info.sta.ssid);
}

void led_blick(void)
{
    led_on(); vTaskDelay(200 / portTICK_PERIOD_MS);
    led_off(); vTaskDelay(200 / portTICK_PERIOD_MS);
    led_on(); vTaskDelay(200 / portTICK_PERIOD_MS);
    led_off(); vTaskDelay(200 / portTICK_PERIOD_MS);
}

void app_main(void)
{
    uint8_t bda[ESP_BD_ADDR_LEN];
    uint8_t data[16];
    char packet[ESP_BD_ADDR_LEN+16];
    char str[64] = {0,};
    int cntTry = 0;
    int ir_retry = 0;
    int ws_retry = 0;

    /* miscellaneous peripheral setup */
    misc_init();

    /* IR Sensor Test */
    // irsensor_init();
    // ir_en_on();
    // while(1) {
    //     irsensor_isDetected();
    //     vTaskDelay(200 / portTICK_PERIOD_MS);
    // }

    switch (deepsleep_get_wakeup()) {
        case ESP_SLEEP_WAKEUP_TIMER:
            if (true) {
                /* IR Sensor */
                irsensor_init();
                ir_en_on();
                while (ir_retry < CNT_IR_CHECK) {
                    if (irsensor_isDetected()) {
                        /* Wi-Fi Connection Check */
                        if (websocket_init()) {
                            /* gatt client and beacon scanner */
                            gattc_init();
                            while (!gattc_isConnected()) {
                                vTaskDelay(500 / portTICK_PERIOD_MS);
                                if (++cntTry > CNT_WAIT_CONNECT) break;
                            }
                            while (gattc_isConnected()) {
                                if (gattc_read(data)) {
                                    gattc_getBDA(bda);

                                    ws_retry = 0;
                                    while (ws_retry++ < CNT_WS_RETRY) {
                                        if (websocket_isConnected()) {
                                            int len = 0;
                                            for (int i = 0; i < 6; i++) {
                                                len += sprintf(str+len, "%02x", bda[i]);
                                            }
                                            for (int i = 0; i < 16; i++) {
                                                len += sprintf(str+len, "%02x", data[i]);
                                            }
                                            printf("%s\n", str);
                                            websocket_send(str, len);
                                            // memcpy(str, bda, sizeof(bda));
                                            // memcpy(str+sizeof(bda), data, sizeof(data));
                                            // websocket_send(str, sizeof(bda)+sizeof(data));
                                            break;
                                        } else {
                                            printf("websocket send retry (%d)\n", ws_retry);
                                            vTaskDelay(1000 / portTICK_PERIOD_MS);
                                        }
                                    }

                                    led_toggle();
                                }
                            }
                            websocket_disconnect();
                            while (websocket_isConnected()) vTaskDelay(10 / portTICK_PERIOD_MS);
                        } else {
                            ESP_LOGE(TAG, "failed to connect wi-fi");
                            break;
                        }
                    } else {
                        ir_retry++;
                        vTaskDelay(200 / portTICK_PERIOD_MS);
                    }
                }
                ir_en_off();
            }
        break;
        case ESP_SLEEP_WAKEUP_EXT1:
            vTaskDelay(1500 / portTICK_PERIOD_MS);
            if (btn_pressed()) {
                led_on();
                if (power) {
                    power = false;

                    vTaskDelay(100 / portTICK_PERIOD_MS);
                    led_off();
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                    led_on();
                    vTaskDelay(100 / portTICK_PERIOD_MS);
                } else {
                    power = true;

                    if (main_wifi_ready()) {
                        vTaskDelay(500 / portTICK_PERIOD_MS);
                        led_off();
                        vTaskDelay(500 / portTICK_PERIOD_MS);
                        led_on();
                        vTaskDelay(500 / portTICK_PERIOD_MS);
                        led_off();
                        vTaskDelay(500 / portTICK_PERIOD_MS);
                        led_on();
                        vTaskDelay(3000 / portTICK_PERIOD_MS);
                        if (btn_pressed()) {
                            /* blufi */
                            blufi_start();
                            while (!blufi_isConnected()) {
                                vTaskDelay(500 / portTICK_PERIOD_MS);
                                if (++cntTry > CNT_WAIT_CONNECT) break;
                                led_toggle();
                            }
                            if (!blufi_isConnected()) {
                                ESP_LOGE(TAG, "failed to connect blufi");
                                power = false;
                                led_off();
                                break;
                            }
                            while (blufi_isConnected()) {
                                vTaskDelay(500 / portTICK_PERIOD_MS);
                                led_on();
                                while (!wifi_isConnected()) {
                                    led_on(); vTaskDelay(200 / portTICK_PERIOD_MS);
                                    led_off(); vTaskDelay(200 / portTICK_PERIOD_MS);
                                    led_on(); vTaskDelay(200 / portTICK_PERIOD_MS);
                                    led_off(); vTaskDelay(200 / portTICK_PERIOD_MS);
                                    vTaskDelay(2000 / portTICK_PERIOD_MS);

                                    if (!blufi_isConnected() || btn_pressed()) {
                                        ESP_LOGE(TAG, "failed to connect wifi");
                                        power = false;
                                        led_off();
                                        break;
                                    }
                                }
                                if (!wifi_isConnected()) {
                                    ESP_LOGE(TAG, "failed to connect wifi 2");
                                    power = false;
                                    led_off();
                                    break;
                                }
                            }
                        }

                    } else {                   
                        /* blufi */
                        blufi_start();
                        while (!blufi_isConnected()) {
                            vTaskDelay(500 / portTICK_PERIOD_MS);
                            if (++cntTry > CNT_WAIT_CONNECT) break;
                            led_toggle();
                        }
                        if (!blufi_isConnected()) {
                            ESP_LOGE(TAG, "failed to connect blufi");
                            power = false;
                            led_off();
                            break;
                        }
                        while (blufi_isConnected()) {
                            vTaskDelay(500 / portTICK_PERIOD_MS);
                            led_on();
                            while (!wifi_isConnected()) {
                                led_on(); vTaskDelay(200 / portTICK_PERIOD_MS);
                                led_off(); vTaskDelay(200 / portTICK_PERIOD_MS);
                                led_on(); vTaskDelay(200 / portTICK_PERIOD_MS);
                                led_off(); vTaskDelay(200 / portTICK_PERIOD_MS);
                                vTaskDelay(2000 / portTICK_PERIOD_MS);

                                if (!blufi_isConnected() || btn_pressed()) {
                                    ESP_LOGE(TAG, "failed to connect wifi");
                                    power = false;
                                    led_off();
                                    break;
                                }
                            }
                            if (!wifi_isConnected()) {
                                ESP_LOGE(TAG, "failed to connect wifi 2");
                                power = false;
                                led_off();
                                break;
                            }
                        }
                    }
                }
                
                led_off();
            }
        break;
        default:
            power = false;
            led_off();
        break;
    }
    
    // if (btn_pressed()) { // user button event check
    //     vTaskDelay(500 / portTICK_PERIOD_MS);
    //     if (btn_pressed()) {
    //         /* blufi */
    //         blufi_start();
    //         while (!blufi_isConnected()) {
    //             vTaskDelay(500 / portTICK_PERIOD_MS);
    //             if (++cntTry > CNT_WAIT_CONNECT) break;
    //             led_toggle();
    //         }
    //         while (blufi_isConnected()) {
    //             vTaskDelay(500 / portTICK_PERIOD_MS);
    //             led_on();
    //             while (!wifi_isConnected()) {
    //                 led_on(); vTaskDelay(200 / portTICK_PERIOD_MS);
    //                 led_off(); vTaskDelay(200 / portTICK_PERIOD_MS);
    //                 led_on(); vTaskDelay(200 / portTICK_PERIOD_MS);
    //                 led_off(); vTaskDelay(200 / portTICK_PERIOD_MS);
    //                 vTaskDelay(2000 / portTICK_PERIOD_MS);
    //             }
    //         }
    //     }
    // } else {
    //     /* IR Sensor */
    //     irsensor_init();
    //     ir_en_on();
    //     while (ir_retry < CNT_IR_CHECK) {
    //         if (irsensor_isDetected()) {
    //             /* Wi-Fi Connection Check */
    //             if (websocket_init()) {
    //                 /* gatt client and beacon scanner */
    //                 gattc_init();
    //                 while (!gattc_isConnected()) {
    //                     vTaskDelay(500 / portTICK_PERIOD_MS);
    //                     if (++cntTry > CNT_WAIT_CONNECT) break;
    //                 }
    //                 while (gattc_isConnected()) {
    //                     if (gattc_read(data)) {
    //                         gattc_getBDA(bda);

    //                         ws_retry = 0;
    //                         while (ws_retry++ < CNT_WS_RETRY) {
    //                             if (websocket_isConnected()) {
    //                                 int len = 0;
    //                                 for (int i = 0; i < 6; i++) {
    //                                     len += sprintf(str+len, "%02x", bda[i]);
    //                                 }
    //                                 for (int i = 0; i < 16; i++) {
    //                                     len += sprintf(str+len, "%02x", data[i]);
    //                                 }
    //                                 printf("%s\n", str);
    //                                 websocket_send(str, len);
    //                                 // memcpy(str, bda, sizeof(bda));
    //                                 // memcpy(str+sizeof(bda), data, sizeof(data));
    //                                 // websocket_send(str, sizeof(bda)+sizeof(data));
    //                                 break;
    //                             } else {
    //                                 printf("websocket send retry (%d)\n", ws_retry);
    //                                 vTaskDelay(1000 / portTICK_PERIOD_MS);
    //                             }
    //                         }

    //                         led_toggle();
    //                     }
    //                 }
    //                 websocket_disconnect();
    //                 while (websocket_isConnected()) vTaskDelay(10 / portTICK_PERIOD_MS);
    //             } else {
    //                 ESP_LOGE(TAG, "failed to connect wi-fi");
    //                 break;
    //             }
    //         } else {
    //             ir_retry++;
    //             vTaskDelay(200 / portTICK_PERIOD_MS);
    //         }
    //     }
    //     ir_en_off();
    // }
    // led_off();
    
    if (power) {
        deepsleep_start(5, BTN_IO);
    } else {
        deepsleep_start(0, BTN_IO);
    }
}
