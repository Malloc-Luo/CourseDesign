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
static volatile uint16_t data systick = 0;

int16_t ModTemperture = 0;
int16_t SetTemperture = 0;
int16_t RefTemperture = 0;
uint8_t SlaveCmd = SET_VAL;

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
        if(isReset == 1) //如果按下重置参考值键，展示重置成功
        {
            LCD_clear();//清屏
            display_reset();
            isReset = 0;
            LCD_clear();
        }
        else
        {
            LCD_display(SetTemperture, ModTemperture);
        }
        
        if(isShowRef | isBelow)//按下展示参考范围键（S2）或者当设定温度超出设定范围时，展示设定温度范围
        {
            LCD_clear();
            display_reftemp(RefTemperture);
            isShowRef = 0;
            isBelow = 0;
            LCD_clear();
        }
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
    
    if (systick % 10 == 1)
    {
        Task_5Hz();
    }
    
    /* 500ms执行一次 */
    if (systick % 25 == 2)
    {
        Task_2Hz();
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

