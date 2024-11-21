//#include "stm32f10x.h" // Device header
//#include "lcd_i2c.h"
//#include "mfrc522.h"
//#include "keypab.h"

//void My_GPIO_Init(void);
//uint8_t CardID[5];

//Keypad_Pin row_pins[ROWS] = {
//	{GPIOB, GPIO_Pin_3},
//	{GPIOB, GPIO_Pin_4},
//	{GPIOB, GPIO_Pin_5},
//	{GPIOB, GPIO_Pin_6}};

//Keypad_Pin col_pins[COLS] = {
//	{GPIOB, GPIO_Pin_7},
//	{GPIOB, GPIO_Pin_8},
//	{GPIOB, GPIO_Pin_9}};

//int main(void)
//{
//	My_GPIO_Init();
//	lcd_init();
//	TM_MFRC522_Init();
//	Keypad_Init(row_pins, col_pins);
//	lcd_gotoxy(0, 1);
//	lcd_Data_Write('v');
//	while (1)
//	{
//		if (TM_MFRC522_Check(CardID) == MI_OK)
//		{
//			lcd_printf(0, 0, "ID: 0x%02X%02X%02X%02X%02X", CardID[0], CardID[1], CardID[2], CardID[3], CardID[4]);
//			GPIO_SetBits(GPIOC, GPIO_Pin_13);
//			delay_ms(2000);
//			GPIO_ResetBits(GPIOC, GPIO_Pin_13);
//		}

//		
//		char c = Keypad_Read();
//		if(c != 0){
//			lcd_gotoxy(0, 1);
//			lcd_Data_Write(c);
//		}
//		delay_ms(50);
//	}
//}

//void My_GPIO_Init(void)
//{
//	GPIO_InitTypeDef gpioInit;

//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	gpioInit.GPIO_Mode = GPIO_Mode_Out_PP;
//	gpioInit.GPIO_Speed = GPIO_Speed_50MHz;
//	gpioInit.GPIO_Pin = GPIO_Pin_13;
//	GPIO_Init(GPIOC, &gpioInit);
//}



//#include "stm32f10x.h"                  // Device header
//#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

//#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
//#include "task.h"                       // ARM.FreeRTOS::RTOS:Core


//static GPIO_InitTypeDef GPIO_InitStructure;

//void Fn_GPIO_Init(void);
//void Fn_RTOS_TaskLed1(void *p);
//void Fn_RTOS_TaskLed2(void *p);


//int main(void)
//{
//	SystemInit();
//	SystemCoreClockUpdate();
//	Fn_GPIO_Init();
//	xTaskCreate(Fn_RTOS_TaskLed1, (const char *) "Red LED Blink", 128, NULL, 1, NULL);
//	xTaskCreate(Fn_RTOS_TaskLed2, (const char *) "Red LED Blink", 128, NULL, 1, NULL);
//	vTaskStartScheduler();
//	return 0;
//}

//void Fn_GPIO_Init(void)
//{
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
//	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_13;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOC, &GPIO_InitStructure);
//} 
//void Fn_RTOS_TaskLed1(void *p)
//{
//	while(1)
//	{
//		GPIOC->ODR ^= GPIO_Pin_13;
//		vTaskDelay(500/portTICK_RATE_MS);
//	}
//}
//void Fn_RTOS_TaskLed2(void *p)
//{
//	while(1)
//	{
//		vTaskDelay(500/portTICK_RATE_MS);
//	}
//}


//#include "stmflash.h"

//int main(){
//	uint32_t a = 0x11223344;
//	Flash_Unlock();
//	Flash_Write_4byte(0x0800FC00, 0x11223344);
//	Flash_Lock();
//	
//	
////	Flash_Unlock();
////	Flash_Erase(0x0800FC00);
////	Flash_Lock();
//	while(1){
//		
//	}
//}


#include "stm32f10x.h"                  // Device header
#include "stm32f10x_gpio.h"             // Keil::Device:StdPeriph Drivers:GPIO

#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "keypab.h"
#include "lcd_i2c.h"
#include "motor.h"

void Fn_Init(void);
void Fn_RTOS_Task_Control(void *p);
void Fn_RTOS_Task_Gui(void *p);
void Fn_RTOS_Task_Finger_sensor(void *p);
void Fn_RTOS_Task_RFID(void *p);


int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
	Fn_Init();
	xTaskCreate(Fn_RTOS_Task_Control,         (const char *) "Fn_RTOS_Task_Control", 128, NULL, 1, NULL);
	xTaskCreate(Fn_RTOS_Task_Gui,             (const char *) "Fn_RTOS_Task_Gui", 128, NULL, 1, NULL);
	xTaskCreate(Fn_RTOS_Task_Finger_sensor,   (const char *) "Fn_RTOS_Task_Finger_sensor", 128, NULL, 1, NULL);
	xTaskCreate(Fn_RTOS_Task_RFID,            (const char *) "Fn_RTOS_Task_RFID", 128, NULL, 1, NULL);
	vTaskStartScheduler();
	return 0;
}

void Fn_Init(void){
	
}


void Fn_RTOS_Task_Control(void *p){
  motor_int();
	while(1){
		GPIOC->ODR ^= GPIO_Pin_13;
		vTaskDelay(500);
	}
}
void Fn_RTOS_Task_Gui(void *p){
	
Keypad_Pin row_pins[ROWS] = {
	{GPIOB, GPIO_Pin_3},
	{GPIOB, GPIO_Pin_4},
	{GPIOB, GPIO_Pin_5},
	{GPIOB, GPIO_Pin_6}};

Keypad_Pin col_pins[COLS] = {
	{GPIOB, GPIO_Pin_7},
	{GPIOB, GPIO_Pin_8},
	{GPIOB, GPIO_Pin_9}};
	Keypad_Init(row_pins, col_pins);
	
	lcd_init();
	
	while(1){
		char c = Keypad_Read();
		if(c != 0){
			lcd_gotoxy(0, 1);
			lcd_Data_Write(c);
		}
		vTaskDelay(50);	
	}
}
void Fn_RTOS_Task_Finger_sensor(void *p){
	while(1){
	
	}
}
void Fn_RTOS_Task_RFID(void *p){
	while(1){
	
	}	
}




