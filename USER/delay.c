#include "delay.h"

static void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	uint16_t PrescalerValue = 0;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	PrescalerValue = (uint16_t) (SystemCoreClock / 1000000) - 1;
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF;
	TIM_TimeBaseStructure.TIM_Prescaler = PrescalerValue;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* TIM3 enable counter */
	TIM_Cmd(TIM3, ENABLE);
}

void delay_init(void)
{
	Timer_Init();
}

void delay_us(u32 nus)
{
	TIM_SetCounter(TIM3, 0);
	while(TIM_GetCounter(TIM3) < nus);
}

void delay_ms(u16 nms)
{
	while (nms) {
		delay_us(1000);
		--nms;
	}
}
