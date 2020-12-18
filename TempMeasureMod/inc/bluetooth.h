#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_
#include "module.h"
#include <reg51.h>

void bt_send_data(uint8_t *cmd, int16_t *dat);

/*
 * 离线检测
 */
extern bit isRCOffline;
extern uint8_t xdata RCOfflineCheckCnt;

#endif
