#ifndef _KEYPAB_H_
#define _KEYPAB_H_
#include "stm32f10x.h"                  // Device header

#define ROWS 4
#define COLS 3

typedef struct {
    GPIO_TypeDef* port;
    uint16_t pin;
} Keypad_Pin;

void Keypad_Init(Keypad_Pin row_pins[], Keypad_Pin col_pins[]);
char Keypad_Read(void);

#endif
