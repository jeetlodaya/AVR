#ifndef UART_H
#define UART_H

#define BAUD 57600
#define UBBR_VALUE (uint16_t)(F_CPU/(16UL*BAUD) - 1)

void UART_Initialize(void);

#endif
