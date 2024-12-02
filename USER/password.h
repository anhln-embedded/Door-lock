#ifndef  _PASSWORD_H_
#define  _PASSWORD_H_

#include "stm32f10x.h"		// Device header
#include "stmflash.h" 

#define PASSWORD_ADDRESS  0x0800FC00
#define ID_CARD_BASE      0x0801F810



void password_save(char pass[]);
uint8_t password_check(char pass[]);
uint32_t getCardNumber(void);
uint8_t erase_id_card(void);
uint8_t add_id_card(uint8_t CardID[5]);
uint8_t check_id_card(uint8_t CardID[5]);

#endif