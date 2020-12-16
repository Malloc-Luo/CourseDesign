#include "module.h"
#include "bluetooth.h"
#include "LCDdisplay.h"

#define RCOFFLINE  0x01 /*遥控离线*/
#define WORKNORMAL 0x02 /*正常工作*/

/*
 * P3.6接LED
 */
sbit LED = P3 ^ 6; 


/**********************FUNCTION***********************
 * @brief: 下位机状态显示
 * @param: sta: 下位机状态，RCOFFLINE WORKNORMAL
 * @note: 如果遥控器离线指示LED以1000ms的周期快闪
 *        如果正常工作LED每隔1000ms闪烁两次
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
                LED = 0;
            }
            else
            {
                LED = 1;
            }
            break;
        default:
            break;
    }
    
    if (++cnt > 8)
    {
        cnt = 0;
    }
}

void Task_10Hz_1()
{    
	
}

void Task_10Hz_2()
{
    /* 离线检测计数, 超过1000ms认为离线 */
    if (RCOfflineCheckCnt++ > 10)
    {
        isRCOffline = 1;
    }
    else
    {
        isRCOffline = 0;
    }
}

void Task_5Hz()
{
    if (isResetRefVal == 1)
    {
        SlaveCmd = RESET;
        isResetRefVal = 0;
    }
    else
    {
        SlaveCmd = SET_VAL;
    }
    
    bt_send_data(&SlaveCmd, &SetTemperture);
		key_set(); //遥控器按键操作
}

/*
 * 500ms 执行一次
 * -> LED状态显示
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