/******************************************************
 * @filename module.h
 * @brief ��ģ���ж�����һЩȫ��ʹ�õı������궨��
 *        ��ģ���ж����˸�ģ����ⲿ�ӿ�
 ******************************************************/
#ifndef __MODULE_H__
#define __MODULE_H__

typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;
typedef unsigned long uint32_t;

typedef signed char int8_t;
typedef signed int  int16_t;
typedef signed long int32_t;

/**
 * �¿�ģ����¶�
 * ����ȡ�����¶� * 10��ת����16λ����
 */
extern uint16_t ModTemperture;
/*
 * �趨���¶�ֵ
 */
extern uint16_t SetTemperture;
/*
 * ��λ��ָ��
 */
#define SET_VAL   0x01
#define ACTUL_VAL 0x02
#define RESET     0x04

extern uint8_t MasterCmd;

extern void Ctrl_Temperture(uint16_t setval);

/*
 * ������
 */

// 10Hz
extern void Task_10Hz_1();
extern void Task_10Hz_2();
// 2Hz
extern void Task_2Hz();
extern void Task_1Hz();


#endif
