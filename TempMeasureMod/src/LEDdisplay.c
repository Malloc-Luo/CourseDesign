#include "LEDdisplay.h"

/*
 * 注意！当按键有改变数字的时候或者发出重置命令的时候
 * 将这两个变量置1
 */
bit isSetValChanged = 0;
bit isResetRefVal = 0;

sbit DAT = P1 ^ 4;
sbit KEY = P1 ^ 5;
sbit CS = P1 ^ 6;
sbit CLK = P1 ^ 7;

/*     
 * led显示 0 1 2 3 4 5 6 7 8 9 - . blank  error 
 */
uint8_t code led_code[] = {0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b, 0x01, 0x80, 0x00, 0x6f};


void led_write(uint8_t cmd, uint8_t dta);
uint8_t get_key_state();
void delay1ms(uint8_t time);
void long_delay(void);
void short_delay(void);

void send_byte(uint8_t out_byte) reentrant
{
    register uint8_t i;
    CS = 0;
    long_delay();

    for(i = 0; i < 8; i++)
    {
        if(out_byte & 0x80)
        {
            DAT = 1;
        }
        else
        {
            DAT = 0;
        }

        CLK = 1;
        short_delay();
        CLK = 0;
        short_delay();
        out_byte = out_byte << 1;
    }

    DAT = 0;
}


int16_t get_setval(void)
{
    uint8_t k_s;
    int16_t ST1, ST2;  //设定温度的中间量
    k_s = get_key_state();
    ST1 = SetTemperture;
    ST2 = SetTemperture;

    switch(k_s)
    {
        case 0x1b:
            isResetRefVal = 1;
            ST1 = SetTemperture;
            ST2 = SetTemperture;
            break;
        case 0x03:
            ST1 += 1;
            break;
        case 0x02:
            ST1 -= 1;
            break;
        case 0x0b:
            ST1 += 10;
            break;
        case 0x0a:
            ST1 -= 10;
            break;
        case 0x13:
            ST1 += 100;
            break;
        case 0x12:
            ST1 -= 100;
            break;
        default:
            break;
    }

    if (ST1 < 3000 && ST1 > RefTemperture)  //判断是否超范围
        SetTemperture = ST1;

    if (ST2 == SetTemperture)  //判断温度有没有变化
        isSetValChanged = 1;
    
    return SetTemperture;
}


void LED_display(int16_t set_temp, int16_t measure_temp)
{
    uint8_t m_t[4], s_t[4];
    register i;
    //判断正负
    m_t[3] = 12;
    s_t[3] = 12;

    if (measure_temp < 0)
    {
        measure_temp = - measure_temp;
        m_t[3] = 10;
    }

    if (set_temp < 0)
    {
        set_temp = - set_temp;
        s_t[3] = 10;
    }

    //获得测量温度和设定温度的个十百千位值
    if(measure_temp / 1000 != 0)
        m_t[3] = measure_temp / 1000;

    measure_temp %= 1000;
    m_t[2] = measure_temp / 100;
    measure_temp %= 100;
    m_t[1] = measure_temp / 10;
    m_t[0] = measure_temp % 10;

    if(set_temp / 1000 != 0)
        s_t[3] = set_temp / 1000;

    set_temp %= 1000;
    s_t[2] = set_temp / 100;
    set_temp %= 100;
    s_t[1] = set_temp / 10;
    s_t[0] = set_temp % 10;

    //将温度数转为led可显示的数
    for(i = 0; i < 4; i++)
    {
        m_t[i] = led_code[m_t[i]];
        s_t[i] = led_code[s_t[i]];
    }

    m_t[1] = m_t[1] | 0x80; //第2位加上小数点
    s_t[1] = s_t[1] | 0x80; //第2位加上小数点

    //将led可显示的数写入led
    for(i = 0 ; i < 4 ; i ++)
    {
        led_write(0x94 + i, m_t[i]);
        led_write(0x90 + i, s_t[i]);
    }
}


void led_write(uint8_t cmd, uint8_t dta)
{
    send_byte(cmd);
    send_byte(dta);
}


uint8_t get_key_state()
{
    register uint8_t i, key_state = 0;
    send_byte(0x15);
    DAT = 1;
    long_delay();

    for(i = 0 ; i < 8 ; i ++)
    {
        CLK = 1;
        short_delay();
        key_state = key_state << 1;

        if(DAT)
        {
            key_state |= 0x01;
        }

        CLK = 0;
        short_delay();
    }

    DAT = 0;
    return key_state;
}


void long_delay(void)
{
    register uint8_t i;
    for(i = 0; i < 0x28; i++);
}


void short_delay(void)
{
    register uint8_t i;
    for(i = 0; i < 6; i++);
}
