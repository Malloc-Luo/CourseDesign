#include <reg51.h>
#include "module.h"
#include "bluetooth.h"

static const uint8_t FRAME_HEADER = 0xa5;
static uint8_t Buffer[2] = { 0x00, 0x00 };
uint8_t isRCOffline = 0;
uint8_t RCOfflineCheckCnt = 0;

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
void send_data(uint16_t temperture, uint8_t instruction)
{
    /* 拆分高8位和低8位 */
    uint8_t Hbit = (uint8_t)((instruction << 4) | ((temperture >> 8) & 0x0f));
    uint8_t Lbit = (uint8_t)(temperture & 0x00ff);
    
    uart_send(FRAME_HEADER);
    uart_send(Hbit);
    uart_send(Lbit);
}

/*
 * 解析指令
 */
static void parsing_instruction()
{
    uint8_t instruct = Buffer[0] >> 4;
    uint16_t temp = (((uint16_t)Buffer[0]) << 8 | (uint16_t)Buffer[1]);
    
    switch (instruct)
    {
        /* 修改设定值 */
        case SET_VAL:
        {
            SetTemperture = temp;
            break;
        }
        /* 重置温度参考值*/
        case RESET:
        {
            RefTemperture = ModTemperture;
            break;
        }
        case ACTUL_VAL:
        default:
            break;
    }
}


void UART_Handler() interrupt 4 using 3
{
    /* 准备接收标志 */
    static uint8_t isReadyRecv = 0;
    /* 接收计数 */
    static uint8_t recvCnt = 0;
    
    if (RI == 1)
    {
        uint8_t buff = SBUF;
        /* 清除离线标志 */
        isRCOffline = 0;
        /*接收到了帧头*/
        if (buff == FRAME_HEADER)
        {
            isReadyRecv = 1;
        }
        
        if (isReadyRecv == 1)
        {
            Buffer[recvCnt++] = buff;
            
            if (recvCnt == 2)
            {
                isReadyRecv = 0;
                recvCnt = 0;
                /* 处理数据 */
                parsing_instruction();
            }
        }
    }
}
