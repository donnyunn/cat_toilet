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

void app_main(void)
{
    gattc_init();
    while(1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}