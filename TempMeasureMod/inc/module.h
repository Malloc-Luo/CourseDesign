/******************************************************
 * @filename module.h
 * @brief 此模块中定义了一些全局使用的变量及宏定义
 *        此模块中定义了各模块的外部接口
 ******************************************************/
#ifndef __MODULE_H__
#define __MODULE_H__
#include <reg51.h>

typedef unsigned char uint8_t;
typedef unsigned int  uint16_t;
typedef unsigned long uint32_t;

typedef signed char int8_t;
typedef signed int  int16_t;
typedef signed long int32_t;

/**
 * 温控模块的温度
 * 将获取到的温度 * 10后转换成16位整形
 */
extern int16_t ModTemperture;

/*
 * 设定的温度值
 */
extern int16_t SetTemperture;

/*
 * 最低参考温度，也就是设定值的最小范围
 */
extern int16_t RefTemperture;

/*
 * 上位机指令
 */
#define SET_VAL   0x01
#define ACTUL_VAL 0x02
#define RESET     0x04

/* 需要发送的指令 */
extern uint8_t MasterCmd;
/* 接收到的指令 */
extern uint8_t RecvMasterCmd;

extern void Ctrl_Temperture(int16_t setval, int16_t actulval);

/*
 * 任务函数
 */

// 10Hz
extern void Task_10Hz_1();
extern void Task_10Hz_2();

extern bit taskFlag_10Hz;

// 5Hz
extern void Task_5Hz();

extern bit taskFlag_5Hz;

// 2Hz
extern void Task_2Hz();

extern bit taskFlag_2Hz;

// 1Hz
extern void Task_1Hz();


#endif
