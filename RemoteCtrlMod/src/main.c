/**********************FILE***************************
 * @filename: main.c
 * @author:   yuqi li
 * @data:     2020.12.11
 * @brief:    
 * 资源分配：
 * T1 ---> UART
 *
 *****************************************************/
#include <reg51.h>
#include "bluetooth.h"
#include "LCDdisplay.h"
/* 系统时钟计数，50ms */
static uint16_t xdata systick = 0;

uint16_t ModTemperture = 0;
uint16_t SetTemperture = 0;
uint16_t RefTemperture = 0;

sfr AUXR = 0x8e;

/* 硬件初始化放在这里面 */
static void hardware_init()
{
    /*
     * 定时器0初始化，溢出周期20ms
     * 允许中断，且为高优先级
     */
    TMOD = 0x01;
    TH0  = 0xb1;
    TL0  = 0xe0;
    TCON = 0x11; 
    IE   = 0x83;
    IP   = 0x01;
    
    /*
     * 串口初始化，波特率9600，1停止位，偶校验
     */
    TMOD = 0x01 | 0x20;
    SCON = 0x50;
    AUXR = 0x00;
    TH1  = 0xfd;
    TL1  = 0xfd;
    IE   = 0x90 | 0x83;
    TR1  = 1;
	  Init_Lcd();
}


void main()
{
    /* 硬件初始化 */
    hardware_init();
    
    for (;;)
		{
			LCD_display(30,265);
		}
}


/**********************FUNCTION***********************
 * @brief: 系统调度器
 * @return: None
 * @note:  在此处调度
 *****************************************************/
static void system_scheduler()
{
    /* 100ms执行一次 */
    if (systick % 5 == 0)
    {
        Task_10Hz_1();
    }
    else if (systick % 5 == 1)
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
    TH0  = 0xb1;
    TL0  = 0xe0;
    
    systick++;
    /*系统调度*/
    system_scheduler();
}

