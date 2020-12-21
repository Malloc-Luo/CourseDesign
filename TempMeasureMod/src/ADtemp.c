#include "ADtemp.h"

/*
 * ��������ʵ�������¶�ֵ
 */
static float Tempf = 0.0f;

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
 * ���õ��ø������¶ȳ���10��ǿ��ת��Ϊ�������󷵻�
 * return (uint16_t)(tempf + 0.5f) * 10.0f;
 */
int16_t get_actulval(void)
{
    
}


