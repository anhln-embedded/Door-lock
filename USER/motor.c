#include "motor.h"

void motor_int(void){
	GPIO_InitTypeDef gpioInit;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin = L298_IN1_PIN ;
	GPIO_Init(L298_IN1_PORT, &gpioInit);
	
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin = L298_IN2_PIN ;
	GPIO_Init(L298_IN2_PORT, &gpioInit);	
	GPIOC->ODR &= ~(GPIO_Pin_13);
	GPIOC->ODR &= ~(GPIO_Pin_14);
}
void motor_up(void){
		GPIOC->ODR |= GPIO_Pin_13;
	GPIOC->ODR &= ~(GPIO_Pin_14);
}
void motor_down(void){
	GPIOC->ODR &= ~(GPIO_Pin_13);
	GPIOC->ODR |= GPIO_Pin_14;
}

void motor_stop(void){
	GPIOC->ODR &= ~(GPIO_Pin_13);
	GPIOC->ODR &= ~(GPIO_Pin_14);
}