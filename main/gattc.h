#ifndef __GATTC_H
#define __GATTC_H

#include "main.h"

bool gattc_read(uint8_t * data);
void gattc_getBDA(uint8_t * buf);
void gattc_scanStart(void);
bool gattc_isConnected(void);
void gattc_init(void);

#endif /* __GATTC_H */