#ifndef _UART_H
#define _UART_H

#include <stdbool.h>

#define USART_TX_ENABLE						USART_CR1_TE
#define USART_RX_ENABLE						USART_CR1_RE

enum BaudRate
{
	BAUD_2400 = 0xD05,
	BAUD_9600 = 0x341,
	BAUD_57600 = 0x8B,
	BAUD_115200 = 0x45
};

enum DMA
{
	TX_DMA_ENABLE = 0,
	RX_DMA_ENABLE,
	BOTH_DMA_ENABLE,
	BOTH_DMA_DISABLE
};

void USART_REGITER_Init(USART_TypeDef* uartPort, uint32_t baud, uint8_t dmaMode, uint8_t enable);
void USART_TransmitByte(USART_TypeDef* uartPort, uint8_t byte);
void USART_TransmitBytes(USART_TypeDef* uartPort, uint8_t* bytes, uint8_t len);
bool USART_RxBufferFull(USART_TypeDef* uartPort);
bool USART_RxIdleLineDetected(USART_TypeDef* uartPort);

#endif //_UART_H
