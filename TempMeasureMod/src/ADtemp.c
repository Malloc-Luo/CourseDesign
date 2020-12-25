#include "ADtemp.h"
#include <math.h>
#include <intRIns.h>

/*
 * 读到的真实浮点型温度值
 */
static float Tf = 0.0f;
/* 测量端 */
static float Aout_1 = 0.0f;
/* 冷端 */
static float Aout_2 = 0.0f;
/* 不要问，我也不知道这是什么 */
int32_t Temp1 = 0;
int32_t Temp2 = 0;
int32_t TempP = 0;
bit isUpdataVal = 0;

sfr CLK_DIV = 0x97;

/* CS5550 接口 */
sbit     CS_CS5550  =  P1 ^ 0;
sbit     MOSI       =  P1 ^ 1;
sbit     MISO       =  P1 ^ 2;
sbit     CLK        =  P1 ^ 3;


/*
 * state = 1制热
 * state = 0降温
 * 风扇控制端：P2.0
 * 加热控制端：P2.1
 */
sbit ColdCtrl = P2 ^ 0;
sbit HeatCtrl = P2 ^ 1;

void ctrl_device(uint8_t state)
{
    switch (state)
    {
        case COLD:
            ColdCtrl = 0;
            HeatCtrl = 1;
            break;
        case HEAT:
            ColdCtrl = 1;
            HeatCtrl = 0;
            break;
        case CLOSE:
        default:
            ColdCtrl = 1;
            HeatCtrl = 1;
            break;
    }
}

/*
 * 延时ms
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
 * 向指定寄存器写入数据 Cmd=0x40+寄存器地址
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
 * 读取指定寄存器数据 Cmd=0x00+寄存器地址 
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
 * CS5550初始化
 * 放在主函数里
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
    wRIte_w(0xe8, 0xffffff);
}

/*
 * 将得到得浮点型温度乘以10后强制转换为浮点数后返回
 * return (uint16_t)(tempf + 0.5f) * 10.0f;
 */
int16_t get_actulval(void)
{
    int16_t itemp = 0;
    
    Aout_1 = Temp1 / (float)0x01000000 * 124.0;
    
    if ((TempP & 0x800000) == 0x800000)
    {
        /* 测量 A/D 转换器第一通道测量mv值 */
        Aout_1 = - 1.0 * Aout_1; 
    }
    
    //Tf = 8.7274f * Aout_1 - 306.6329f;
    //Tf = 8.796f * Aout_1 - 308.836f;
    Tf = 8.4583f * Aout_1 - 296.193;
    itemp = (int16_t)(Tf * 10.0f + 0.5f);
    
    return itemp;
}
