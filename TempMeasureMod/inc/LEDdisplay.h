/******************************************************
 * @filename LEDdisplay.h
 * @brief 
 * ��ģ���������������LED��ʾ�ĳ���
 * 
 ******************************************************/
#ifndef __LED_DISPLAY_H__
#define __LED_DISPLAY_H__
#include "module.h"

/*
 * setval: �趨�¶�ֵ
 * actulval: ʵ���¶�ֵ
 */
extern void LED_display(uint16_t setval, uint16_t actulval);

/*
 * ��ȡ�趨�¶�ֵ
 */
extern uint16_t get_setval(void);

#endif
