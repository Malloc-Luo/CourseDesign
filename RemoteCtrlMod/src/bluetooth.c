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
 * �������ݺ�ָ��
 */
void Send_data(uint16_t temperture, uint8_t instruction)
{
    /* ��ָ�8λ�͵�8λ */
    uint8_t Hbit = (uint8_t)((instruction << 4) | (temperture >> 8));
    uint8_t Lbit = (uint8_t)(temperture & 0x00ff);
    
    uart_send(Hbit);
    uart_send(Lbit);
}