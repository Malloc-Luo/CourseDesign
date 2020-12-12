#ifndef _BLUETOOTH_H_
#define _BLUETOOTH_H_
#include "module.h"
#include <reg51.h>

void uart_send(uint8_t);
void send_data(uint16_t temperture, uint8_t instruction);
void recv_data(uint16_t *, uint8_t *);

#endif
