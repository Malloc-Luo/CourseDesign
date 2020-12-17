#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_
#include "module.h"
#include <reg51.h>

void send_data(uint16_t temperture, uint8_t instruction);

/*
 * ÀëÏß¼ì²éÒ£¿ØÆ÷
 */
extern uint8_t isRCOffline;
extern uint8_t RCOfflineCheckCnt;

#endif
