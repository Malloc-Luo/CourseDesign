#include "module.h"
#include "bluetooth.h"

#define RCOFFLINE  0x01 /*ң������*/
#define WORKNORMAL 0x02 /*��������*/

/*
 * P3.6��LED
 */
sbit LED = P3 ^ 6; 


/**********************FUNCTION***********************
 * @brief: ��λ��״̬��ʾ
 * @param: sta: ��λ��״̬��RCOFFLINE WORKNORMAL
 * @note: ���ң��������ָʾLED��1000ms�����ڿ���
 *        �����������LEDÿ��1000ms��˸����
 *****************************************************/
static void LED_state(uint8_t sta)
{
    static uint8_t cnt = 0;
    switch (sta)
    {
        case RCOFFLINE:
            LED = cnt % 2;
            break;
        case WORKNORMAL:
            if (cnt == 2 || cnt == 4)
            {
                LED = 1;
            }
            else
            {
                LED = 0;
            }
            break;
        default:
            break;
    }
    
    if (++cnt > 5)
    {
        cnt = 0;
    }
}

void Task_10Hz_1()
{    
}

void Task_10Hz_2()
{
    /* ���߼�����, ����1000ms��Ϊ���� */
    if (RCOfflineCheckCnt++ > 10)
    {
        isRCOffline = 1;
    }
}

/*
 * ִ������500ms
 */
void Task_2Hz()
{
    static uint8_t state = 0;
    
    if (isRCOffline == 1)
    {
        state = RCOFFLINE;
    }
    else
    {
        state = WORKNORMAL;
    }
    
    LED_state(state);
}

void Task_1Hz()
{
}