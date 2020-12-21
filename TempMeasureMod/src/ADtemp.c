#include "ADtemp.h"
#include <math.h>
#include <intRIns.h>

/*
 * ��������ʵ�������¶�ֵ
 */
static float Tf = 0.0f;
/* ������ */
static float Aout_1 = 0.0f;
/* ��� */
static float Aout_2 = 0.0f;
/* ��Ҫ�ʣ���Ҳ��֪������ʲô */
int32_t Temp1 = 0;
int32_t Temp2 = 0;
int32_t TempP = 0;
bit isUpdataVal = 0;

sfr CLK_DIV = 0x97;

/* CS5550 �ӿ� */
sbit     CS_CS5550  =  P1 ^ 0;
sbit     MOSI       =  P1 ^ 1;
sbit     MISO       =  P1 ^ 2;
sbit     CLK        =  P1 ^ 3;
/* ����ѡ���� */
sbit     BB         =  P3 ^ 2;
sbit     AA         =  P3 ^ 3;

/*
 * state = 1����
 * state = 0����
 * ���ȿ��ƶˣ�P1.0, P1.1
 * ���ȿ��ƶˣ�P1.2, P1.3
 */
void ctrl_device(uint8_t state)
{
    if (state == 1)
    {
        /* �رշ��� */
        P1 &= 0xfc;
        /* �������� */
        P1 |= 0x0c;
    }
    else if (state == 0)
    {
        /* �ر����� */
        P1 &= 0xf3;
        /* �������� */
        P1 |= 0x03;
    }
    else
    {
        /* ȫ���ص���*/
        P1 &= 0xf0;
    }
}

/*
 * ��ʱms
 */
void delay(int ms)	  
{
    register uint16_t j, g;

    for(j = 0; j < ms; j++)
    {
        for(g = 0; g < 335; g++)
        {
            _nop_();
            _nop_();
            _nop_();
            _nop_();
        }
    }
}


void outb(uint8_t dat)
{
    register uint8_t i;

    for(i = 0; i < 8; i++)
    {
        CLK = 0;
        if(dat & 0x80)
        {            
            MOSI = 1;
        }
        else
        {            
            MOSI = 0;
        }
        CLK = 1;
        dat <<= 1;
    }
    CLK = 0;
}

uint8_t inb()
{
    register uint8_t temp, i;

    for (i = 0; i < 8; i++)
    {  
        MOSI = (i == 7) ? 0 : 1;
        CLK = 0;
        temp <<= 1;
        CLK = 1;

        if (MISO)
        {            
            temp |= 1;
        }
        else 
        {
            temp &= 0xFE;
        }
        _nop_();
    }

    CLK = 0;
    return temp;
}
/*
 * ��ָ���Ĵ���д������ Cmd=0x40+�Ĵ�����ַ
 */
static void wRIte_w(uint8_t cmd, uint32_t val)
{
    CS_CS5550 = 0;
    outb(cmd);
    outb(val >> 16);
    outb(val >> 8);
    outb(val);
    CS_CS5550 = 1;
}

/* 
 * ��ȡָ���Ĵ������� Cmd=0x00+�Ĵ�����ַ 
 */
uint32_t read_w(uint8_t cmd)
{
    uint32_t temp;

    CS_CS5550 = 0;
    outb(cmd);
    temp = ((uint32_t)inb() << 16);
    temp |= ((uint32_t)inb() << 8);
    temp |= inb();
    CS_CS5550 = 1;

    return temp;
}

/*
 * CS5550��ʼ��
 * ������������
 */
void CS5550Init()
{
    wRIte_w(0x80, 0xffffff);
    delay(1);
    wRIte_w(0xff, 0xfffffe);
    delay(1);
    wRIte_w(0x40, 0x000011);
    delay(1);           
    wRIte_w(0xe8, 0xffffff);
    delay(1);
    wRIte_w(0x4a, 0x000138);
    delay(2);
    
    CLK_DIV = 0x40;
    EA = 1;
    wRIte_w(0x40, 0x000011);
    delay(1);
    BB = 0;
    AA = 0;
    wRIte_w(0xe8, 0xffffff);
}

/*
 * ���õ��ø������¶ȳ���10��ǿ��ת��Ϊ�������󷵻�
 * return (uint16_t)(tempf + 0.5f) * 10.0f;
 */
int16_t get_actulval(void)
{
    int16_t itemp = 0;
    
    Aout_1 = Temp1 / (float)0x01000000 * 124.0;
    
    if ((TempP & 0x800000) == 0x800000)
    {
        /* ���� A/D ת������һͨ������mvֵ */
        Aout_1 = - 1.0 * Aout_1; 
    }
    
    Tf = 8.7274f * Aout_1 - 306.6329f;
    itemp = (int16_t)(Tf * 10.0f + 0.5f);
    
    return itemp;
}
