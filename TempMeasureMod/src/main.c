/**********************FILE***************************
 * @filename: main.c
 * @author:   yuqi li
 * @data:     2020.12.11
 * @brief:    
 * 资源分配：
 * T1 ---> UART
 * P1.0, P1.1, P1.2, P1.3 制冷制热设备控制
 * P0.0, P0.1, P0.2, P0.3 数码管显示及键盘设置
 * P2, P0.4, P0.5 AD转换获取温度
 *
 *****************************************************/
#include <reg51.h>
#include "bluetooth.h"
/* 系统时钟计数，50ms */
static uint16_t xdata systick = 0;

uint16_t ModTemperture = 0;
uint16_t SetTemperture = 0;
uint16_t RefTemperture = 0;

/* 硬件初始化放在这里面 */
static void hardware_init()
{
    /*
     * 定时器0初始化，溢出周期50ms
     * 允许中断，且为高优先级
     */
    TMOD = 0x01;
    TH0  = 0x3c;
    TL0  = 0xb0;
    TCON = 0x11; 
    IE   = 0x83;
    IP   = 0x01;
    
    /*
     * your cod
     */
}


void main()
{
    /* 硬件初始化 */
    hardware_init();
    
    for (;;);
}

/**********************FUNCTION***********************
 * @brief: 系统调度器
 * @return: None
 * @note:  在此处调度
 *****************************************************/
void system_scheduler()
{
    /* 100ms执行一次 */
    if (systick % 2)
    {
        Task_10Hz_1();
    }
    else
    {
        Task_10Hz_2();
    }
    
    /* 500ms执行一次 */
    if (systick % 10)
    {
        Task_2Hz();
    }
    /* 1000ms执行一次 */
    if (systick % 20)
    {
        Task_1Hz();
    }
}

void timer0() interrupt 1 using 1
{
    systick++;
    /*系统调度*/
    system_scheduler();
}

