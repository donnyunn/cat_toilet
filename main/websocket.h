#ifndef __WEBSOCKET_H
#define __WEBSOCKET_H

#include "main.h"

bool websocket_isConnected(void);
void websocket_send(char * buf, int len);
void websocket_connect(void);
void websocket_disconnect(void);
bool websocket_init(void);

#endif /* __WEBSOCKET_H */