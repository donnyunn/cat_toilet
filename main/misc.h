#ifndef __MISC_H
#define __MISC_H

#include "main.h"

#define IR_EN_IO GPIO_NUM_32
#define LED_IO GPIO_NUM_26
#define BTN_IO GPIO_NUM_27

bool btn_pressed(void);
void ir_en_off(void);
void ir_en_on(void);
void led_off(void);
void led_on(void);
void led_toggle(void);
void misc_init(void);

#endif /* __MISC_H */