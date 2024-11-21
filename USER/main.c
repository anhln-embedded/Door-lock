#include "stm32f10x.h" // Device header
#include "lcd_i2c.h"
#include "mfrc522.h"
#include "keypab.h"

void My_GPIO_Init(void);
uint8_t CardID[5];

Keypad_Pin row_pins[ROWS] = {
	{GPIOB, GPIO_Pin_3},
	{GPIOB, GPIO_Pin_4},
	{GPIOB, GPIO_Pin_5},
	{GPIOB, GPIO_Pin_6}};

Keypad_Pin col_pins[COLS] = {
	{GPIOB, GPIO_Pin_7},
	{GPIOB, GPIO_Pin_8},
	{GPIOB, GPIO_Pin_9}};

int main(void)
{
	My_GPIO_Init();
	lcd_init();
	TM_MFRC522_Init();
	Keypad_Init(row_pins, col_pins);
	lcd_gotoxy(0, 1);
	lcd_Data_Write('v');
	while (1)
	{
		if (TM_MFRC522_Check(CardID) == MI_OK)
		{
			lcd_printf(0, 0, "ID: 0x%02X%02X%02X%02X%02X", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			delay_ms(2000);
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		}

		
		char c = Keypad_Read();
		if(c != 0){
			lcd_gotoxy(0, 1);
			lcd_Data_Write(c);
		}
		delay_ms(50);
	}
}

void My_GPIO_Init(void)
{
	GPIO_InitTypeDef gpioInit;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
	gpioInit.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOC, &gpioInit);
}
