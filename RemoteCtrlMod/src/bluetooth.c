#include <reg51.h>
#include "module.h"
#include "bluetooth.h"


void uart_send(uint8_t byte)
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
void Send_data(uint16_t temperture, uint8_t instruction)
{
    /* 拆分高8位和低8位 */
    uint8_t Hbit = (uint8_t)((instruction << 4) | (temperture >> 8));
    uint8_t Lbit = (uint8_t)(temperture & 0x00ff);
    
    uart_send(Hbit);
    uart_send(Lbit);
}