#ifndef __USART2_H
#define __USART2_H	 
#include "stm32f10x.h"                  // Device header

 

#define USART2_MAX_RECV_LEN		400				
#define USART2_MAX_SEND_LEN		400					
#define USART2_RX_EN 			1					

extern u8  USART2_RX_BUF[USART2_MAX_RECV_LEN]; 		
extern u8  USART2_TX_BUF[USART2_MAX_SEND_LEN]; 	
extern vu16 USART2_RX_STA;   					

void usart2_init(u32 bound);			
void u2_printf(char* fmt,...);
#endif













