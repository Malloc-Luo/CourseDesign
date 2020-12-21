#include "ADtemp.h"

/*
 * 读到的真实浮点型温度值
 */
static float Tempf = 0.0f;

/*
 * state = 1制热
 * state = 0降温
 * 风扇控制端：P1.0, P1.1
 * 加热控制端：P1.2, P1.3
 */
void ctrl_device(uint8_t state)
{
    if (state == 1)
    {
        /* 关闭风扇 */
        P1 &= 0xfc;
        /* 开启制热 */
        P1 |= 0x0c;
    }
    else if (state == 0)
    {
        /* 关闭制热 */
        P1 &= 0xf3;
        /* 开启风扇 */
        P1 |= 0x03;
    }
    else
    {
        /* 全部关掉！*/
        P1 &= 0xf0;
    }
}

/*
 * 将得到得浮点型温度乘以10后强制转换为浮点数后返回
 * return (uint16_t)(tempf + 0.5f) * 10.0f;
 */
int16_t get_actulval(void)
{
    
}


