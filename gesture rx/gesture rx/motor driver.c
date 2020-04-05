#include "motor driver.h"

inline void MOTOR_Initialize()
{
	L293D_DDR |= (1 << L293D_EN_L12) | (1 << L293D_EN_R12) | (1 << L293D_L1) | (1 << L293D_L2) | (1 << L293D_R1) | (1 << L293D_R2);
	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) ; //phase corrected pwm 
	TCCR0B = (1 << CS00) | (1 << CS02); //prescale 1024
}

void MOTOR_Forward(uint8_t left_speed,uint8_t right_speed)
{
	L293D_PORT |= (1 << L293D_L1) | (1 << L293D_R1);
	L293D_PORT &= ~((1 << L293D_L2) | (1 << L293D_R2));
	L_SPEED = left_speed;
	R_SPEED = right_speed;
}

void MOTOR_Backward(uint8_t left_speed,uint8_t right_speed)
{
	L293D_PORT |= (1 << L293D_L2) | (1 << L293D_R2);
	L293D_PORT &= ~((1 << L293D_L1) | (1 << L293D_R1));
	L_SPEED = left_speed;
	R_SPEED = right_speed;
}

void MOTOR_HardLeft(uint8_t left_speed,uint8_t right_speed)
{
	L293D_PORT |= (1 << L293D_L2) | (1 << L293D_R1);
	L293D_PORT &= ~((1 << L293D_L1) | (1 << L293D_R2));
	L_SPEED = left_speed;
	R_SPEED = right_speed;
}

void MOTOR_HardRight(uint8_t left_speed,uint8_t right_speed)
{
	L293D_PORT |= (1 << L293D_L1) | (1 << L293D_R2);
	L293D_PORT &= ~((1 << L293D_L2) | (1 << L293D_R1));	
	L_SPEED = left_speed;
	R_SPEED = right_speed;
}

void MOTOR_SoftFrontRight(uint8_t left_speed)
{
	L293D_PORT |= (1 << L293D_L1);
	L293D_PORT &= ~((1 << L293D_L2) | (1 << L293D_R1) | (1 << L293D_R2));
	L_SPEED = left_speed;
}

void MOTOR_SoftFrontLeft(uint8_t right_speed)
{
	L293D_PORT |= (1 << L293D_R1);
	L293D_PORT &= ~((1 << L293D_L1) | (1 << L293D_L2) | (1 << L293D_R2));
	R_SPEED = right_speed;
}

void MOTOR_SoftBackRight(uint8_t right_speed)
{
	L293D_PORT |= (1 << L293D_R2);
	L293D_PORT &= ~((1 << L293D_R1) | (1 << L293D_L1) | (1 << L293D_L2));
	R_SPEED = right_speed;
}

void MOTOR_SoftBackLeft(uint8_t left_speed)
{
	L293D_PORT |= (1 << L293D_L2);
	L293D_PORT &= ~((1 << L293D_L1) | (1 << L293D_R1) | (1 << L293D_R2));
	L_SPEED = left_speed;
}

void MOTOR_Stop(void)
{
	L293D_PORT &= ~((1 << L293D_L1) | (1 << L293D_R1) | (1 << L293D_L2) | (1 << L293D_R2));
	
}