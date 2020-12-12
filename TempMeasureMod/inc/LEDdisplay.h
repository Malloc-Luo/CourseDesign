/******************************************************
 * @filename LEDdisplay.h
 * @brief 
 * 此模块包含按键交互及LED显示的程序
 * 
 ******************************************************/
#ifndef __LED_DISPLAY_H__
#define __LED_DISPLAY_H__
#include "module.h"

/*
 * setval: 设定温度值
 * actulval: 实际温度值
 */
extern void LED_display(uint16_t setval, uint16_t actulval);

/*
 * 获取设定温度值
 */
extern uint16_t get_setval(void);

/*
 * 如果按键状态发生改变将这个值置1
 * 后续由用户手动清零
 */
extern uint8_t isSetValChanged;

/*
 * 是否重置参考值(最低值)，若重置参考值的按键被按下则改位置1
 * 使用后手动清零
 */
extern uint8_t isResetRefVal;

#endif
