#include <reg51.h>
#include "module.h"
#include "bluetooth.h"

static const uint8_t xdata FRAME_HEADER = 0xa5;
static uint8_t data Buffer[4] = { 0x00, 0x00, 0x00, 0x00 };
bit isRCOffline = 1;
bit ForceSynchronFlag = 0;
uint8_t RCOfflineCheckCnt = 21;

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
    uint8_t * ptr = (uint8_t *)dat;
    /* 数据帧头0xa5 */
    uart_send(FRAME_HEADER);
    /* 指令帧 */
    uart_send(*cmd);
    /* 数据帧 */
    uart_send(*(ptr + 0));
    uart_send(*(ptr + 1));
}

/*
 * 解析指令
 */
static void parsing_instruction()
{
    uint8_t instruct = Buffer[1];
    int16_t temp =  *(int16_t *)(Buffer + 2);
    
    if ((instruct & 0xf0) == 0xf0)
    {
        ForceSynchronFlag = 1;
        instruct &= 0x0f;
    }
    else
    {
        ForceSynchronFlag = 0;
    }
    
    switch (instruct)
    {
        /* 修改设定值 */
        case SET_VAL:
        {
            if (temp <= MAX_TEMP && temp >= RefTemperture)
            {
                SetTemperture = temp;
            }
            break;
        }
        /* 重置温度参考值*/
        case RESET:
        {
            RefTemperture = temp;
            break;
        }
        case ACTUL_VAL:
        {
            ModTemperture = temp;
            break;
        }
        default:
            break;
    }
}

/* 准备接收标志 */
static volatile bit isReadyRecv = 0;
/* 接收计数 */
static volatile uint8_t recvCnt = 0;

void UART_Handler() interrupt 4 using 3
{
    if (RI == 1)
    {
        uint8_t buff = SBUF;
        /* 清除离线标志 */
        RCOfflineCheckCnt = 0;
        /*接收到了帧头*/
        if (buff == FRAME_HEADER)
        {
            isReadyRecv = 1;
        }
        
        if (isReadyRecv == 1)
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
