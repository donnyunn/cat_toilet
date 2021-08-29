#ifndef __BLUFI_H
#define __BLUFI_H

#include "main.h"

esp_ble_adv_params_t * get_adv_params(void);
bool wifi_isConnected(void);
bool blufi_isConnected(void);
void blufi_start(void);

#endif /* __BLUFI_H */