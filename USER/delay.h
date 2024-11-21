#ifndef DELAY_H_
#define DELAY_H_

#include "stm32f10x.h"

//void Delay_Init(void);
//void Delay_Us(uint32_t u32DelayInUs);
//void Delay_Ms(uint32_t u32DelayInMs);

void delay_init(void);
void delay_ms(u16 nms);
void delay_us(u32 nus);

#endif
