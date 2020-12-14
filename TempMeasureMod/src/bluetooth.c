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
 * �������ݺ�ָ��
 */
void send_data(uint16_t temperture, uint8_t instruction)
{
    /* ��ָ�8λ�͵�8λ */
    uint8_t Hbit = (uint8_t)((instruction << 4) | ((temperture >> 8) & 0x0f));
    uint8_t Lbit = (uint8_t)(temperture & 0x00ff);
    
    uart_send(FRAME_HEADER);
    uart_send(Hbit);
    uart_send(Lbit);
}

/*
 * ����ָ��
 */
static void parsing_instruction()
{
    uint8_t instruct = Buffer[0] >> 4;
    uint16_t temp = (((uint16_t)Buffer[0]) << 8 | (uint16_t)Buffer[1]);
    
    switch (instruct)
    {
        /* �޸��趨ֵ */
        case SET_VAL:
        {
            SetTemperture = temp;
            break;
        }
        /* �����¶Ȳο�ֵ*/
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
    /* ׼�����ձ�־ */
    static uint8_t isReadyRecv = 0;
    /* ���ռ��� */
    static uint8_t recvCnt = 0;
    
    if (RI == 1)
    {
        uint8_t buff = SBUF;
        /* ������߱�־ */
        isRCOffline = 0;
        /*���յ���֡ͷ*/
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
                /* �������� */
                parsing_instruction();
            }
        }
    }
}
