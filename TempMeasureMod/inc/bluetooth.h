#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_
#include "module.h"
#include <reg51.h>

void bt_send_data(uint8_t *cmd, int16_t *dat);

/*
 * 离线检测
 */

/* 是否离线 */
extern bit isRCOffline;
/* 上一次的离线状态 */
extern bit LastTimeOfflineStatus;
extern uint16_t xdata RCOfflineCheckCnt;

/*
 * RC连接检测，该变量计数减小
 */
/* 最大计数 */
#define MAX_CNT 11
extern uint8_t xdata RCConnectCnt;

#endif
