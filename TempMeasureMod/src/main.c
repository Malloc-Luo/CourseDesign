/**********************FILE***************************
 * @filename: main.c
 * @author:   yuqi li
 * @data:     2020.12.11
 * @brief:    
 * ��Դ���䣺
 * T1 ---> UART
 * P1.0, P1.1, P1.2, P1.3 ���������豸����
 * P0.0, P0.1, P0.2, P0.3 �������ʾ����������
 * P2, P0.4, P0.5 ADת����ȡ�¶�
 *
 *****************************************************/
#include <reg51.h>
#include "bluetooth.h"
/* ϵͳʱ�Ӽ�����50ms */
static uint16_t xdata systick = 0;

uint16_t ModTemperture = 0;
uint16_t SetTemperture = 0;
uint16_t RefTemperture = 0;

/* Ӳ����ʼ������������ */
static void hardware_init()
{
    /*
     * ��ʱ��0��ʼ�����������50ms
     * �����жϣ���Ϊ�����ȼ�
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
    /* Ӳ����ʼ�� */
    hardware_init();
    
    for (;;);
}

/**********************FUNCTION***********************
 * @brief: ϵͳ������
 * @return: None
 * @note:  �ڴ˴�����
 *****************************************************/
void system_scheduler()
{
    /* 100msִ��һ�� */
    if (systick % 2)
    {
        Task_10Hz_1();
    }
    else
    {
        Task_10Hz_2();
    }
    
    /* 500msִ��һ�� */
    if (systick % 10)
    {
        Task_2Hz();
    }
    /* 1000msִ��һ�� */
    if (systick % 20)
    {
        Task_1Hz();
    }
}

void timer0() interrupt 1 using 1
{
    systick++;
    /*ϵͳ����*/
    system_scheduler();
}

