#ifndef __LCD_DISPLAY_H__
#define __LCD_DISPLAY_H__
#include "module.h"

/*
 * 1602LCD�¶���ʾ
 * setval: �趨ֵ  actulval: ʵ��ֵ ��Χ0~999����ֳ���������
 */
extern void LCD_display(uint16_t setval, uint16_t actulval); 

/*
 * ��ȡ�¶��趨ֵ
 */
extern uint16_t get_setval(void);

/*
 * �������״̬�����ı佫���ֵ��1
 * �������û��ֶ�����
 */
extern uint8_t isSetValChanged;

/*
 * �Ƿ����òο�ֵ(���ֵ)�������òο�ֵ�İ������������λ��1
 * ʹ�ú��ֶ�����
 */
extern uint8_t isResetRefVal;

//LCD1602��ʼ��
void Init_Lcd(void);
#endif

