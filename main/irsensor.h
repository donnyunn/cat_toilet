
#ifndef __ADC_H
#define __ADC_H

#include "main.h"

void adc_init(void);
int adc_get_result(void);

bool irsensor_isDetected(void);
void irsensor_init(void);

#endif /* __DEEPSLEEP_H */