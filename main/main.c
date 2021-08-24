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

void app_main(void)
{
    uint8_t bda[6];
    uint8_t data[16];

    /* gatt client and beacon scanner */
    gattc_init();

    /* IR Sensor */
    irsensor_init();

    while(1)
    {
        if (gattc_isConnected()) {
            vTaskDelay(10 / portTICK_PERIOD_MS);
            if (gattc_read(data)) {
                gattc_getBDA(bda);
                for (int i = 0; i < 6; i++) {
                    printf("%02x", bda[i]);
                }
                printf(" ");
                for (int i = 0; i < 16; i++) {
                    printf("%02x", data[i]);
                }
                printf("\n");
            }
        } else {
            if (irsensor_isDetected()) {
                printf("enable\n");
            }
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }
    }
}
