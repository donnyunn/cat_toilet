#include "misc.h"

volatile bool led_state, ir_en_state;

bool btn_pressed(void)
{
    bool ret;
    if (gpio_get_level(BTN_IO) != 0) ret = true;
    else ret = false;
    return ret;
}

void ir_en_off(void)
{
    gpio_set_level(IR_EN_IO, 0);
}

void ir_en_on(void)
{
    gpio_set_level(IR_EN_IO, 1);
    vTaskDelay(100 / portTICK_PERIOD_MS);
}

void led_off(void)
{
    gpio_set_level(LED_IO, 0);
    led_state = false;
}

void led_on(void)
{
    gpio_set_level(LED_IO, 1);
    led_state = true;
}

void led_toggle(void)
{
    if (led_state) led_off();
    else led_on();
}

void misc_init(void)
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL<<LED_IO);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    io_conf.pin_bit_mask = (1ULL<<IR_EN_IO);
    gpio_config(&io_conf);

    // io_conf.intr_type = GPIO_INTR_POSEDGE;
    io_conf.pin_bit_mask = (1ULL<<BTN_IO);
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pull_down_en = 1;
    gpio_config(&io_conf);

    led_off();
}
