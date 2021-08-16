#ifndef __GATTC_H
#define __GATTC_H

#include "main.h"

bool gattc_read(uint8_t * data);
bool gattc_isConnected(void);
void gattc_init(void);

#endif /* __GATTC_H */