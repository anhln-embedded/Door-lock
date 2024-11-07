#include "keypab.h"

static const Keypad_Pin *g_row_pins;
static const Keypad_Pin *g_col_pins;

static void Enable_GPIO_Clock(GPIO_TypeDef *GPIOx);

char keymap[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}
};

void Keypad_Init(Keypad_Pin row_pins[], Keypad_Pin col_pins[]){
	g_row_pins = row_pins;
	g_col_pins = col_pins;
	
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	for(int row = 0; row < ROWS; row++){
		Enable_GPIO_Clock(row_pins[row].port);
		GPIO_InitStructure.GPIO_Pin = row_pins[row].pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(row_pins[row].port, &GPIO_InitStructure);
	}
	
	for(int col = 0; col < COLS; col ++){
		Enable_GPIO_Clock(col_pins[col].port);
		GPIO_InitStructure.GPIO_Pin = col_pins[col].pin;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
		GPIO_Init(col_pins[col].port, &GPIO_InitStructure);		
	}
	
}
char Keypad_Read(void){

	for(int col = 0; col < COLS; col ++){
		GPIO_ResetBits(g_col_pins[col].port, g_col_pins[col].pin);
		for(int row = 0; row < ROWS; row++){
			if(GPIO_ReadInputDataBit(g_row_pins[row].port, g_row_pins[row].pin) == 0){
				GPIO_SetBits(g_col_pins[col].port, g_col_pins[col].pin);
//				while(GPIO_ReadInputDataBit(g_row_pins[row].port, g_row_pins[row].pin) == 0);
				return keymap[row][col];
			}
		}
		GPIO_SetBits(g_col_pins[col].port, g_col_pins[col].pin);
	}
	return 0;
}
static void Enable_GPIO_Clock(GPIO_TypeDef *GPIOx){
    if (GPIOx == GPIOA) 
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    } 
		else if (GPIOx == GPIOB)
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    } 
		else if (GPIOx == GPIOC) 
		{
			RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    } 
}
