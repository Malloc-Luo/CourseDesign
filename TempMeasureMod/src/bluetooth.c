#include <reg51.h>
#include "module.h"
#include "bluetooth.h"
#include "LEDdisplay.h"

/* 帧头 */
#define FRAME_HEADER 0xa5

static uint8_t Buffer[4] = { 0x00, 0x00, 0x00, 0x00 };
bit isRCOffline = 1;
bit LastTimeOfflineStatus = 0;
uint16_t xdata RCOfflineCheckCnt = 11;
uint8_t xdata RCConnectCnt = 0;

static void uart_send(uint8_t byte)
{
    ES = 0;
    SBUF = byte;
    while (TI == 0);
    TI = 0;
    ES = 1;
}

/*
 * 发送数据和指令
 */
void bt_send_data(uint8_t *cmd, int16_t *dat)
{
    uint8_t *ptr = (uint8_t *)dat;
    uart_send(FRAME_HEADER);
    uart_send(*cmd);
    uart_send(*(ptr + 0));
    uart_send(*(ptr + 1));
}

/*
 * 解析指令
 */
static void parsing_instruction()
{
    int16_t temp = *(int16_t *)(Buffer + 2);
    RecvMasterCmd = Buffer[1];
    
    /* 
     * 这个时候在强制同步，不应该修改接收数据 
     * 或者离线的时候，也不应该修改接收数据
     */
    if (!RCConnectCnt && !isRCOffline)
    {
        switch (RecvMasterCmd)
        {
            /* 修改设定值，如果设定值没有主动改变 
             * 并且没有发错（本来是想加校验的）
             */
            case SET_VAL:
            {
                if (!isSetValChanged && temp <= MAX_TEMP && temp >= RefTemperture)
                {
                    SetTemperture = temp;
                }
                break;
            }
            /* 重置温度参考值*/
            case RESET:
            {
                RefTemperture = ModTemperture;
                isResetRefVal = 1;
                break;
            }
            case ACTUL_VAL:
            default:
                break;
        }
    }
}

/* 准备接收标志 */
static bit isReadyRecv = 0;
/* 接收计数 */
static volatile uint8_t recvCnt = 0;

void UART_Handler() interrupt 4 using 3
{
    if (RI)
    {
        uint8_t buff = SBUF;
        /* 清除离线标志 */
        RCOfflineCheckCnt = 0;
        /*接收到了帧头*/
        if (buff == FRAME_HEADER)
        {
            isReadyRecv = 1;
        }
        
        if (isReadyRecv)
        {
            Buffer[recvCnt++] = buff;
            
            if (recvCnt == 4)
            {
                isReadyRecv = 0;
                recvCnt = 0;
                /* 处理数据 */
                parsing_instruction();
            }
        }
        
        RI = 0;
    }
}
