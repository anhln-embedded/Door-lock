
#include "stm32f10x.h"		// Device header
#include "stm32f10x_gpio.h" // Keil::Device:StdPeriph Drivers:GPIO
#include "FreeRTOS.h"		// ARM.FreeRTOS::RTOS:Core
#include "task.h"			// ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"
#include "keypab.h"
#include "lcd_i2c.h"
#include "motor.h"
#include "delay.h"
#include "usart1.h"
#include "usart2.h"
#include "as608.h"
#include "password.h"
#include "mfrc522.h"

EventGroupHandle_t xEventGroup;

#define EVENT_OPEN_DOOR (1 << 0)
#define EVENT_ADD_NEW_FINGER_PRINT (1 << 1)

typedef enum
{
	WAIT_FOR_UNLOCK,
	CHANGE_PASSWORD,
	ADD_FINGERPRINT,
	ADD_RFID_CARD,
	DELETE_FINGERPRINT,
	DELETE_RFID_CARD,
} system_stt;

static enum {
	ENTER_CHANGE_PASS,
	ENTER_OLD_PASS,
	ENTER_NEW_PASS,
} change_pass = ENTER_CHANGE_PASS;

static enum {
	ENTER_ADD_FINGERPRINT,
	ENTER_PASSWORD,
	ENRER_ADD_NEW_FINGERPRINT,
} add_fingerprint = ENTER_ADD_FINGERPRINT;

static enum {
	ENTER_DELETE_FINGERPRINT,
	ENTER_PASSWORD_DELETE_FINGERPRINT,
	FINISH_DELETE_FINGERPRINT,
} delete_fingerprint = ENTER_DELETE_FINGERPRINT;

volatile system_stt SYSTEM_STATUS = ENTER_CHANGE_PASS;

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
	xTaskCreate(Fn_RTOS_Task_Control, (const char *)"Fn_RTOS_Task_Control", 128, NULL, 1, NULL);
	xTaskCreate(Fn_RTOS_Task_Gui, (const char *)"Fn_RTOS_Task_Gui", 128, NULL, 1, NULL);
	xTaskCreate(Fn_RTOS_Task_Finger_sensor, (const char *)"Fn_RTOS_Task_Finger_sensor", 128, NULL, 1, NULL);
	xTaskCreate(Fn_RTOS_Task_RFID, (const char *)"Fn_RTOS_Task_RFID", 128, NULL, 1, NULL);
	vTaskStartScheduler();
	return 0;
}

void Fn_Init(void)
{
	xEventGroup = xEventGroupCreate();
	uint8_t CardID[5] = {0x32, 0x5c, 0xd7, 0x00, 0xb9};
	//	add_id_card(CardID);
	//	while(1);
}

void Fn_RTOS_Task_Control(void *p)
{
	motor_int();
	const EventBits_t xBitsToWaitFor = EVENT_OPEN_DOOR;
	while (1)
	{
		const EventBits_t xEventBits = xEventGroupWaitBits(xEventGroup, xBitsToWaitFor, pdTRUE, pdFALSE, portMAX_DELAY);
		if ((xEventBits & EVENT_OPEN_DOOR) != 0)
		{
			motor_up();
			delay_ms(2000);
			motor_down();
			xEventGroupClearBits(xEventGroup, EVENT_OPEN_DOOR);
			Delete_LCD();
			lcd_printf(0, 0, "WAIT FOR UNLOCK");
		}
	}
}
void Fn_RTOS_Task_Gui(void *p)
{

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
	char input;
	char password_input[10];
	uint8_t password_input_index = 0;
	SYSTEM_STATUS = WAIT_FOR_UNLOCK;
	EventBits_t xEventBits;
	while (1)
	{
		switch (SYSTEM_STATUS)
		{
		case WAIT_FOR_UNLOCK:
			lcd_printf(0, 0, "WAIT FOR UNLOCK");
			input = Keypad_Read_Until();
			if (input == '*')
			{
				password_input[0] = 0; /*reset password*/
				SYSTEM_STATUS = CHANGE_PASSWORD;
				change_pass = ENTER_OLD_PASS;
				Delete_LCD();
				lcd_printf(0, 0, "CHANGE PASSWORD");
			}
			else if (input == '#')
			{
				Delete_LCD();
				password_input_index = 0;
				if (password_check(password_input))
				{
					lcd_printf(0, 1, "UNLOCKED");
					xEventGroupSetBits(xEventGroup, EVENT_OPEN_DOOR);
				}
				else
				{
					lcd_printf(0, 1, "WRONG PASSWORD");
				}
			}
			else
			{
				password_input[password_input_index] = input;
				lcd_printf(password_input_index, 1, "*              ");
				password_input_index++;
				password_input[password_input_index] = 0;
			}
			break;
		case CHANGE_PASSWORD:
			input = Keypad_Read_Until();
			if (input == '*')
			{
				password_input[0] = 0; /*reset password*/
				change_pass = ENTER_CHANGE_PASS;
				password_input_index = 0;
				SYSTEM_STATUS = ADD_FINGERPRINT;
				Delete_LCD();
				lcd_printf(0, 0, "ADD FINGERPRINT");
			}
			else if (input == '#')
			{
				switch (change_pass)
				{
				case ENTER_CHANGE_PASS:
					Delete_LCD();
					lcd_printf(0, 0, "ENTER OLD PASS");
					change_pass = ENTER_OLD_PASS;
					break;
				case ENTER_OLD_PASS:

					password_input_index = 0;
					if (password_check(password_input))
					{
						Delete_LCD();
						lcd_printf(0, 0, "ENTER NEW PASS");
						change_pass = ENTER_NEW_PASS;
					}
					else
					{
						Delete_LCD();
						lcd_printf(0, 1, "WRONG PASSWORD");
						change_pass = ENTER_CHANGE_PASS;
					}
					break;
				case ENTER_NEW_PASS:
					password_save(password_input);
					Delete_LCD();
					lcd_printf(0, 0, "PASSWORD CHANGED");
					change_pass = ENTER_CHANGE_PASS;
					break;
				default:
					break;
				}
			}
			else
			{
				password_input[password_input_index] = input;
				lcd_printf(password_input_index, 1, "*              ");
				password_input_index++;
				password_input[password_input_index] = 0;
			}
			break;
		case ADD_FINGERPRINT:
			input = Keypad_Read_Until();
			if (input == '*')
			{
				password_input[0] = 0;
				add_fingerprint = ENTER_ADD_FINGERPRINT;
				password_input_index = 0;
				SYSTEM_STATUS = ADD_RFID_CARD;
			}
			else if (input == '#')
			{
				switch (add_fingerprint)
				{
				case ENTER_ADD_FINGERPRINT:
					Delete_LCD();
					lcd_printf(0, 0, "ENTER PASSWORD");
					add_fingerprint = ENTER_PASSWORD;
					break;
				case ENTER_PASSWORD:
					password_input_index = 0;
					if (password_check(password_input))
					{
						Delete_LCD();
						lcd_printf(0, 0, "ADD NEW FINGERPRINT");
						add_fingerprint = ENRER_ADD_NEW_FINGERPRINT;
					}
					else
					{
						Delete_LCD();
						lcd_printf(0, 1, "WRONG PASSWORD");
						add_fingerprint = ENTER_ADD_FINGERPRINT;
					}
					break;
				case ENRER_ADD_NEW_FINGERPRINT:
					break;
				default:
					break;
				}
			}
			else
			{
				password_input[password_input_index] = input;
				lcd_printf(password_input_index, 1, "*              ");
				password_input_index++;
				password_input[password_input_index] = 0;
			}
			break;
		case ADD_RFID_CARD:
			Delete_LCD();
			lcd_printf(0, 0, "ADD RFID CARD");
			input = Keypad_Read_Until();
			if (input == '*')
			{
				SYSTEM_STATUS = DELETE_FINGERPRINT;
				Delete_LCD();
				lcd_printf(0, 0, "DEL FINGERPRINT");
			}
			break;
		case DELETE_FINGERPRINT:
			input = Keypad_Read_Until();
			if (input == '*')
			{
				SYSTEM_STATUS = DELETE_RFID_CARD;
			}
			else if (input == '#')
			{
				switch (delete_fingerprint)
				{
				case ENTER_DELETE_FINGERPRINT:
					Delete_LCD();
					lcd_printf(0, 0, "ENTER PASSWORD");
					delete_fingerprint = ENTER_PASSWORD_DELETE_FINGERPRINT;
					break;
				case ENTER_PASSWORD_DELETE_FINGERPRINT:
					if (password_check(password_input))
					{
						Delete_LCD();
						lcd_printf(0, 0, "FINISH DELETE");
						delete_fingerprint = FINISH_DELETE_FINGERPRINT;
					}
					else
					{
						Delete_LCD();
						lcd_printf(0, 1, "WRONG PASSWORD");
						delete_fingerprint = ENTER_DELETE_FINGERPRINT;
					}
					break;
				case FINISH_DELETE_FINGERPRINT:
					break;
				default:
					break;
				}
			}
			else
			{
				password_input[password_input_index] = input;
				lcd_printf(password_input_index, 1, "*              ");
				password_input_index++;
				password_input[password_input_index] = 0;
			}
			break;
		case DELETE_RFID_CARD:
			Delete_LCD();
			lcd_printf(0, 0, "DEL RFID CARD");
			input = Keypad_Read_Until();
			if (input == '*')
			{
				SYSTEM_STATUS = WAIT_FOR_UNLOCK;
			}
			break;
		default:
			break;
		}
	}
}
void Fn_RTOS_Task_Finger_sensor(void *p)
{
	Serial_Init();
	usart2_init(57600);
	uint8_t res;
	uint16_t pageStart = 0;
	uint16_t pageNum = 2000;
	SearchResult result;
	while (1)
	{
		switch (SYSTEM_STATUS)
		{
		case WAIT_FOR_UNLOCK:
			res = PS_GetImage();
			if (res != 0)
			{
				Serial_Printf("Khong the thu thap anh dau van tay\n");
				break;
			}
			Serial_Printf("Anh dau van tay thu thap thanh cong\n");
			res = PS_GenChar(CharBuffer1);
			if (res != 0)
			{
				Serial_Printf("Khong the tao dac trung tu anh\n");
				break;
			}
			Serial_Printf("Dac trung tu anh tao thanh cong\n");
			res = PS_Search(CharBuffer1, pageStart, pageNum, &result);
			if (res == 0)
			{
				xEventGroupSetBits(xEventGroup, EVENT_OPEN_DOOR);
				Delete_LCD();
				lcd_printf(0, 1, "FINGERPRINT OK");
			}
			else
			{
				Delete_LCD();
				lcd_printf(0, 1, "FINGERPRINT FAIL");
				Serial_Printf("Khong tim thay dau van tay\n");
			}
			break;
		case ADD_FINGERPRINT:
			if (add_fingerprint == ENRER_ADD_NEW_FINGERPRINT)
			{
				res = PS_GetImage();
				if (res != 0)
				{
					Serial_Printf("Khong the thu thap anh dau van tay\n");
					break;
				}
				else
				{
					Serial_Printf("Anh dau van tay thu thap thanh cong\n");
				}
				res = PS_GenChar(CharBuffer1);
				if (res != 0)
				{
					Serial_Printf("Khong the tao dac trung tu anh\n");
					break;
				}
				Serial_Printf("Dac trung tu anh tao thanh cong\n");
				res = PS_RegModel();
				if (res != 0)
				{
					Serial_Printf("Khong the dang ky dac trung\n");
					break;
				}
				Serial_Printf("Dang ky dac trung thanh cong\n");
				res = PS_StoreChar(CharBuffer1, 1);
				if (res != 0)
				{
					Serial_Printf("Khong the luu dac trung\n");
					Delete_LCD();
					lcd_printf(0, 1, "FINGERPRINT FAIL");
					break;
				}
				else
				{
					Serial_Printf("Luu dac trung thanh cong\n");
					Delete_LCD();
					lcd_printf(0, 1, "FINGERPRINT OK");
					add_fingerprint = ENTER_ADD_FINGERPRINT;
				}
			}
			break;
		case DELETE_FINGERPRINT:
			if (delete_fingerprint == FINISH_DELETE_FINGERPRINT)
			{
				res = PS_Empty();
				if (res == 0)
				{
					Delete_LCD();
					lcd_printf(0, 1, "DELETE OK");
					delete_fingerprint = ENTER_DELETE_FINGERPRINT;
				}
				else
				{
					Delete_LCD();
					lcd_printf(0, 1, "DELETE FAIL");
				}
			}
			break;
		default:
			break;
		}
		vTaskDelay(200 / portTICK_RATE_MS);
	}
}
void Fn_RTOS_Task_RFID(void *p)
{
	uint8_t CardID[5];
	TM_MFRC522_Init();
	while (1)
	{
		switch (SYSTEM_STATUS)
		{
		case WAIT_FOR_UNLOCK:
			if (TM_MFRC522_Check(CardID) == MI_OK)
			{
				if (check_id_card(CardID))
				{
					xEventGroupSetBits(xEventGroup, EVENT_OPEN_DOOR);
					Delete_LCD();
					lcd_printf(0, 1, "RFID OK");
				}
				else
				{
					Delete_LCD();
					lcd_printf(0, 1, "RFID FAIL");
				}
			}
			break;
		case ADD_RFID_CARD:
			break;
		case DELETE_RFID_CARD:
			break;
		default:
			break;
		}
		vTaskDelay(200 / portTICK_RATE_MS);
	}
}
