#include "module.h"
#include "bluetooth.h"
#include "LEDdisplay.h"
#include "ADtemp.h"

#define RCOFFLINE  0x01 /*遥控离线*/
#define WORKNORMAL 0x02 /*正常工作*/

bit taskFlag_10Hz = 0;
bit taskFlag_5Hz = 0;
bit taskFlag_2Hz = 0;

/*
 * P3.6接LED
 */
sbit LED = P3 ^ 6; 

/*
 * 发送数据的地址
 */
int16_t * SendTempePtr;

/**********************FUNCTION***********************
 * @brief: 下位机状态显示
 * @param: sta: 下位机状态，RCOFFLINE WORKNORMAL
 * @note: 如果遥控器离线指示LED以1000ms的周期快闪
 *        如果正常工作LED每隔1000ms闪烁两次
 *****************************************************/
static void LED_state(uint8_t sta)
{
    static volatile uint8_t cnt = 0;
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

/*
 * 执行频率5Hz
 * 主要用于蓝牙串口发送指令，及按键获取
 */
void Task_5Hz()
{
    if (!RCConnectCnt)
    {
        get_setval();
        /*
         * 如果是重置参考值或者接到党中央发来的指示：
         * 则把参考值发回党中央，且标志位清零
         */
        if (isResetRefVal)
        {
            isResetRefVal = 0;
            RefTemperture = ModTemperture;
            SendTempePtr = &RefTemperture;
            MasterCmd = RESET;
        }
        /*
         * 如果设定值发生改变，则向党中央同步设定值
         */
        else if (isSetValChanged)
        {
            isSetValChanged = 0;
            MasterCmd = SET_VAL;
            SendTempePtr = &SetTemperture;
        }
        else
        {
            MasterCmd = ACTUL_VAL;
            SendTempePtr = &ModTemperture;
        }
    }
    else
    {
        /*
         * 连接初始状态强制同步，顺序不能改变
         */
        if (RCConnectCnt >= 8)
        {
            MasterCmd = ACTUL_VAL;
            SendTempePtr = &ModTemperture;
        }
        else if (RCConnectCnt >= 4)
        {
            MasterCmd = RESET;
            SendTempePtr = &RefTemperture;
        }
        else
        {
            MasterCmd = SET_VAL;
            SendTempePtr = &SetTemperture;
        }
        
        MasterCmd |= 0xf0;
        RCConnectCnt--;
    }
    
    bt_send_data(&MasterCmd, SendTempePtr);
}


void Task_10Hz_1()
{
    /* 记录上一次的离线状态 */
    LastTimeOfflineStatus = isRCOffline;
    
    /* 离线检测计数, 超过1000ms认为离线 */
    if (RCOfflineCheckCnt++ > 10)
    {
        isRCOffline = 1;
    }
    else
    {
        isRCOffline = 0;
    }
    
    /* 从离线状态切换到在线状态 */
    if (LastTimeOfflineStatus && !isRCOffline)
    {
        RCConnectCnt = MAX_CNT;
    }
    
    TempP = read_w(0x0e);
    Temp1 = read_w(0x16);
}

/*
 * 执行周期500ms
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
    Ctrl_Temperture(SetTemperture, ModTemperture);
}
