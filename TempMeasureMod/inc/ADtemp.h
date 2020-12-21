#ifndef __ADTEMP_H__
#define __ADTEMP_H__
#include "module.h"

/*
 * 获取当前温度值 0~999
 */
extern int16_t get_actulval(void); 

/*
 * 制冷、制热
 * state = 0 制冷
 * state = 1 制热
 * state = 2 关闭设备，自由冷却
 */

#define COLD 0x00
#define HEAT 0x01
#define CLOSE 0x02
extern void ctrl_device(uint8_t state);

#endif
