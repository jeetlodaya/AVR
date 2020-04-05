#ifndef NRF24_H
#define NRF24_H

#include <avr/io.h>
#include "spi.c"
#include <util/delay.h>

#define W_TX_PAYLOAD 0xa0
#define R_RX_PAYLOAD 0x61
#define W_ACK_PAYLOAD 0xa8
#define STATUS 0x07
#define NOP 0xff

#define TX_DS 5
#define RX_DR 6

#define CE_PORT PORTB
#define CE 1

inline void NRF24_Initialize(void); 
void NRF24_WriteByte(uint8_t address,uint8_t data);
uint8_t NRF24_ReadByte(uint8_t address);
void NRF24_WriteByteS(uint8_t address,uint8_t *arr,uint8_t size);
void NRF24_ReadByteS(uint8_t address,uint8_t *arr,uint8_t size);
void NRF24_WriteTxFifo(uint8_t *arr,uint8_t size);
void NRF24_ReadRxFifo(uint8_t *arr,uint8_t size);
void NRF24_WriteAckPayload(uint8_t *arr,uint8_t size);

#endif