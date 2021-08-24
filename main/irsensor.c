/* ADC1 Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include "irsensor.h"

#define TAG "IRSENSOR"

#define SAMPLE_NUM 64
#define DETECTION_REFVAL 1200

uint32_t irsensor_getResult(void)
{
    uint32_t value = 0;
    for (int i = 0; i < SAMPLE_NUM; i++) {
        value += adc1_get_raw((adc1_channel_t)ADC_CHANNEL_6);
    }
    value /= SAMPLE_NUM;

    return value;
}

bool irsensor_isDetected(void)
{
    uint32_t irsensor_value = irsensor_getResult();
    // ESP_LOGI(TAG, "IR Sensor Value: %d\n", irsensor_value);
    if (irsensor_value > DETECTION_REFVAL) {
        return true;
    } else {
        return false;
    }
}

void irsensor_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_12);
    adc1_config_channel_atten(ADC_CHANNEL_6, ADC_ATTEN_DB_11);
}

