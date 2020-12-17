#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_
#include "module.h"
#include <reg51.h>

void bt_send_data(uint8_t *cmd, uint16_t *dat);

/*
 * 离线检查遥控器
 */
extern volatile bit isRCOffline;
extern uint8_t RCOfflineCheckCnt;

#endif
