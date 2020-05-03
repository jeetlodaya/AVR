#ifndef IR_NEC_H
#define IR_NEC_H

#include <avr/io.h>
#include <stdbool.h>

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


struct nec_packet{
	uint8_t addr_l;
	uint8_t addr_h;
	uint8_t cmd;
	uint8_t cmd_inv;
	};
	
enum nec_status{
	PACKET_RECVD = 0,
	PACKET_RECVD_N_REPEAT_RECVNG,
	IDLE,
	TIMING_ERR,
	CHKSM_ERR
	};	
	
volatile struct nec_packet nec_current_packet = {.addr_l = 0,.addr_h = 0,.cmd = 0,.cmd_inv = 0};	
volatile enum nec_status nec_current_status = IDLE;
volatile uint8_t nec_data_counter = 0;

static volatile uint8_t nec_current_state = 0;
static volatile uint8_t repeat_timer = 0;
		
void IR_Initialize(void);
void IR_DeInitialize(void);

#endif