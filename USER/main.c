#include "stm32f10x.h"                  // Device header
#include "lcd_i2c.h"
#include "mfrc522.h"

void My_GPIO_Init(void);
uint8_t CardID[5];

int main(void)
{
	My_GPIO_Init();
	lcd_init();
	TM_MFRC522_Init();
	
	while(1)
	{
		if (TM_MFRC522_Check(CardID) == MI_OK) {
			lcd_printf(0, 0, "ID: 0x%02X%02X%02X%02X%02X", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			Delay_Ms(2000);
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}
	}
}

void My_GPIO_Init(void) {
	GPIO_InitTypeDef gpioInit;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioInit.GPIO_Mode=GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed=GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin=GPIO_Pin_13;
	GPIO_Init(GPIOC, &gpioInit);
}
