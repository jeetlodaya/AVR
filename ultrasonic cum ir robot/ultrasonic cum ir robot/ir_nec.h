#ifndef IR_NEC_H
#define IR_NEC_H

#define IR_TSOP_DDR DDRB
#define IR_TSOP_PIN 0

#define IR_START_TIME_H 30000
#define IR_START_TIME_L 25000
#define IR_LOW_TIME_H 2400
#define IR_LOW_TIME_L 1800
#define	IR_HIGH_TIME_H 4800
#define IR_HIGH_TIME_L 4000
#define IR_REPEAT1_TIME_H 24000
#define IR_REPEAT1_TIME_L 20000
#define IR_REPEAT2_TIME_H 65000
#define IR_REPEAT2_TIME_L 62000

#define IR_ICR_BUFFER_SIZE 20

struct IR_nec_packet_TypeDef{
	uint8_t addr_l;
	uint8_t addr_h;
	uint8_t cmd;
	uint8_t cmd_inv;
};

struct IR_circular_q_TypeDef{
	uint16_t buffer[IR_ICR_BUFFER_SIZE];
	uint8_t read_index;
	uint8_t write_index;
};

enum IR_circular_q_status_TypeDef{
	EMPTY = 0,
	OK,
	OVERFLOW
};

enum  IR_nec_status_TypeDef{
	PACKET_RECVD = 0,
	PACKET_RECVD_N_REPEAT_RECVNG,
	IDLE,
	TIMING_ERR,
	CHKSM_ERR
};

extern volatile struct IR_nec_packet_TypeDef IR_nec_current_packet;
extern volatile struct IR_circular_q_TypeDef IR_icr_buffer;
extern volatile enum  IR_nec_status_TypeDef IR_nec_current_status;
extern volatile enum IR_circular_q_status_TypeDef IR_icr_buffer_status;
extern volatile bool IR_nec_new_data;

void IR_Initialize(void);
void IR_DeInitialize(void);
void IR_ProcessData(void);
void IR_WriteBuffer(uint16_t data);
uint16_t IR_ReadBuffer(void);
void IR_FlushBuffer(void);

#endif