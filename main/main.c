#include "main.h"

void app_main(void)
{
    gattc_demo_init();
    while(1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}