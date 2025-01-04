/*
  ******************************************************************************
  * @file		motor.h                                                            *
  * @author	Luu Ngoc Anh                                                       *
  * @date		08/09/2024                                                         *
  ******************************************************************************
*/
	
#ifndef __MOTOR_H__
#define __MOTOR_H__

#ifdef __cplusplus
 extern "C" {
#endif
#include "stm32f10x.h"                  // Device header

#define L298_IN1_PORT GPIOC 
#define L298_IN1_PIN GPIO_Pin_13; 

#define L298_IN2_PORT GPIOC 
#define L298_IN2_PIN GPIO_Pin_14; 

void motor_int(void);
void motor_up(void);
void motor_down(void);
void motor_stop(void);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/
