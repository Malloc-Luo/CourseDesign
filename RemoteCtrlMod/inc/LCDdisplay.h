#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__
#include "module.h"

/*
 * 1602LCD温度显示
 * setval: 设定值  actulval: 实际值 范围0~999，拆分成三个整数
 */
extern void LCD_display(int16_t setval, int16_t actulval); 

/*
 * 获取温度设定值
 */
extern int16_t get_setval(void);

/*
 * 如果按键状态发生改变将这个值置1
 * 后续由用户手动清零
 */
extern bit isSetValChanged;

/*
 * 是否重置参考值(最低值)，若重置参考值的按键被按下则改位置1
 * 使用后手动清零
 */
extern bit isResetRefVal;

//判断是否重置，若重置则置1，显示重置成功后置0
extern bit isReset;

//判断是否展示最低参考值
extern bit isShowRef;

//判断是否超出设定范围
extern bit isBelow;

//显示重置成功
extern void display_reset(void);

//展示最低参考值
extern void display_reftemp(int16_t reftemp);

//按键操作
extern void key_set(void);

//LCD清屏
extern void LCD_clear(void);


//LCD1602初始化
void Init_Lcd(void);
#endif

