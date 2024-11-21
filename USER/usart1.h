#ifndef __USART1_H
#define __USART1_H

#include "stm32f10x.h"                  // Device header

#define PRINTF_BUFFER_SIZE 128 
#define BUFFER_SIZE 64        

extern char rx_buffer[BUFFER_SIZE];  
extern uint8_t Serial_RxFlag;      

void Serial_Init(void);                                
void Serial_SendByte(uint8_t Byte);                    
void Serial_SendArray(uint8_t *Array, uint16_t Length); 
void Serial_SendString(char *String);                   
void Serial_SendNumber(uint32_t Number, uint8_t Length);        
void Serial_Printf(char *format, ...);

uint8_t Serial_GetRxFlag(void);

char* Serial_GetRxData(void);
void clear_rx_buffer(void);


#endif
