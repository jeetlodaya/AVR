#ifndef MOTORDRIVER_H
#define MOTORDRIVER_H

#include <avr/io.h>

#define L293D_DDR DDRD
#define L293D_PORT PORTD
#define L293D_EN_L12 5
#define L293D_EN_R12 6
#define L293D_L1 2
#define L293D_L2 3
#define L293D_R1 4
#define L293D_R2 7

#define L_SPEED OCR0B
#define R_SPEED OCR0A

inline void MOTOR_Initialize(void);
void MOTOR_Forward(uint8_t left_speed,uint8_t right_speed);
void MOTOR_Backward(uint8_t left_speed,uint8_t right_speed);
void MOTOR_HardLeft(uint8_t left_speed,uint8_t right_speed);
void MOTOR_HardRight(uint8_t left_speed,uint8_t right_speed);
void MOTOR_SoftFrontRight(uint8_t left_speed);
void MOTOR_SoftFrontLeft(uint8_t right_speed);
void MOTOR_SoftBackRight(uint8_t right_speed);
void MOTOR_SoftBackLeft(uint8_t left_speed);
void MOTOR_Stop(void);
#endif


