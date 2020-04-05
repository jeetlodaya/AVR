#include "nrf24.h"

inline void NRF24_Initialize( void )
{
	SPI_Initialize();
	NRF24_WriteByte(0x00,0x78);//pw =1
	_delay_ms(2);
	CE_PORT |= (1 << CE);
	_delay_us(10);
	NRF24_WriteByte(0x00,0x7a); //mask int pw = 1 and tx mode
	NRF24_WriteByte(0x01,0x00); //AA off
	NRF24_WriteByte(0x02,0x01);
	NRF24_WriteByte(0x04,0x13);
	NRF24_WriteByte(0x11,0x0c); //12 bytes	
}

void NRF24_WriteByte(uint8_t address,uint8_t data)
{	
	SPI_CsClear();
	SPI_Transciever(0x20 | address);
	SPI_Transciever(data);	
	SPI_CsSet();
}

uint8_t NRF24_ReadByte(uint8_t address)
{
	uint8_t data;
	SPI_CsClear();
	SPI_Transciever(address);
	data = SPI_Transciever(0);
	SPI_CsSet();
	return data;
}

void NRF24_WriteByteS(uint8_t address,uint8_t *arr,uint8_t size)
{
	SPI_CsClear();
	SPI_Transciever(0x20 | address);
	do 
	{
		SPI_Transciever(*(arr++));
		
	} while (--size);
	SPI_CsSet();
}

void NRF24_ReadByteS(uint8_t address,uint8_t *arr,uint8_t size)
{
	SPI_CsClear();
	SPI_Transciever(address);
	do
	{
		*(arr++) =	SPI_Transciever(0);
		
	} while (--size);
	SPI_CsSet();
}

void NRF24_WriteTxFifo(uint8_t *arr,uint8_t size)
{
	uint8_t status;
	
	SPI_CsClear();
	SPI_Transciever(W_TX_PAYLOAD);
	do
	{
		SPI_Transciever(*(arr++));
		
	} while (--size);
	SPI_CsSet();
	
	do
	{
		SPI_CsClear();
		status = SPI_Transciever(NOP);
		SPI_CsSet();
		
	} while (!(status & (1 << TX_DS)));
	
	
	//status &= ~(1 << RX_DR);
	NRF24_WriteByte(STATUS,status);
}

void NRF24_ReadRxFifo(uint8_t *arr,uint8_t size)
{
	uint8_t status;
	
	do
	{
		SPI_CsClear();
		status = SPI_Transciever(NOP);
		SPI_CsSet();
		
	} while (!(status & (1 << RX_DR)));
	
	SPI_CsClear();
	SPI_Transciever(R_RX_PAYLOAD);
	do
	{
		*(arr++) = SPI_Transciever(0);
		
	} while (--size);
	SPI_CsSet();
	
	//status &= ~(1 << RX_DR);
	NRF24_WriteByte(STATUS,status);
}


void NRF24_WriteAckPayload(uint8_t *arr,uint8_t size)
{
	
}

