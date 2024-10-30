#ifndef AS608_H
#define AS608_H

#include "stm32f10x.h"

// UART configuration
void AS608_UART_Init(void);

// Fingerprint sensor functions
uint8_t AS608_AddFingerprint(void);
uint8_t AS608_SearchFingerprint(void);
uint8_t AS608_DeleteFingerprint(uint16_t id);

#endif // AS608_H