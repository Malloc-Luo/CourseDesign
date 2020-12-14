/**********************FILE***************************
 * @filename: main.c
 * @author:   yuqi li
 * @data:     2020.12.11
 * @brief:    
 * ��Դ���䣺
 * T1 ---> UART
 * P1.0, P1.1, P1.2, P1.3 ���������豸����
 * P1.4, P1.5, P1.6, P1.7 �������ʾ����������
 * P2, P0.4, P0.5 ADת����ȡ�¶�
 * P3.6��LED��������������������LEDÿ��1000ms��˸һ��
 *
 *****************************************************/
#include <reg51.h>
#include "bluetooth.h"
/* ϵͳʱ�Ӽ�����20ms */
static uint16_t xdata systick = 0;

uint16_t ModTemperture = 0;
uint16_t SetTemperture = 0;
uint16_t RefTemperture = 0;

sfr AUXR = 0x8e;

/* Ӳ����ʼ������������ */
static void hardware_init()
{
    /*
     * ��ʱ��0��ʼ�����������20ms
     * �����жϣ���Ϊ�����ȼ�
     */
    TMOD = 0x01;
    TH0  = 0xb1;
    TL0  = 0xe0;
    TCON = 0x11; 
    IE   = 0x83;
    IP   = 0x01;
    
    /*
     * ���ڳ�ʼ����������9600��1ֹͣλ��żУ��
     */
    TMOD = 0x01 | 0x20;
    SCON = 0x50;
    AUXR = 0x00;
    TH1  = 0xfd;
    TL1  = 0xfd;
    IE   = 0x90 | 0x83;
    TR1  = 1;
    
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
    if (systick % 5 == 0)
    {
        Task_10Hz_1();
    }
    else if (systick % 5 == 1)
    {
        Task_10Hz_2();
    }
    
    /* 500msִ��һ�� */
    if (systick % 25)
    {
        Task_2Hz();
    }
    /* 1000msִ��һ�� */
    if (systick % 50)
    {
        Task_1Hz();
    }
}

void timer0() interrupt 1 using 1
{
    TH0  = 0xb1;
    TL0  = 0xe0;
    
    systick++;
    /*ϵͳ����*/
    system_scheduler();
}

